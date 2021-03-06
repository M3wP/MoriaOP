{ Returns the value for any given object }
//[psect(store$code)] \
function item_value(item: treasure_type): integer;
	function search_list(x1, x2: integer): integer;
		var
		i1, i2: integer;

		begin
		i1:= 0;
		i2:= 0;
		repeat
			i1:= i1 + 1;
			with object_list[i1] do
				if ((tval = x1) and (subval = x2)) then
					i2:= cost;
			until ((i1 = max_objects) or (i2 > 0));
		search_list:= i2
		end;

	begin
	with item do
		begin
		item_value:= cost;
		if (tval in [20, 21, 22, 23, 30, 31, 32, 33, 34, 35, 36]) then
			begin { Weapons and armor }
			if (index(name, '^') > 0) then
				item_value:= search_list(tval, subval) * number
			else if (tval in [20, 21, 22, 23]) then
				begin
				if (tohit < 0) then
					item_value:= 0
				else if (todam < 0) then
					item_value:= 0
				else if (toac < 0) then
					item_value:= 0
				else
					item_value:= (cost + (tohit + todam + toac) * 100) * number
				end
			else
				begin
				if (toac < 0) then
					item_value:= 0
				else
					item_value:= (cost + toac * 100) * number
				end
			end
		else if (tval in [10, 11, 12, 13]) then
			begin { Ammo }
			if (index(name, '^') > 0) then
				item_value:= search_list(tval, 1) * number
			else
				begin
				if (tohit < 0) then
					item_value:= 0
				else if (todam < 0) then
					item_value:= 0
				else if (toac < 0) then
					item_value:= 0
				else
					item_value:= (cost + (tohit + todam + toac) * 10) * number
				end
			end
		else if (tval in [70, 71, 75, 76, 80]) then
			begin { Potions, Scrolls, and Food }
			if (index(name, '|') > 0) then
				case tval of
					70, 71:
						item_value:= 20;
					75, 76:
						item_value:= 20;
					80:
						item_value:= 1;
					else
					end
			end
		else if (tval in [40, 45]) then
			begin { Rings and amulets }
			if (index(name, '|') > 0) then
				case tval of
					40:
						item_value:= 45;
					45:
						item_value:= 45;
					else
					end
			else if (index(name, '^') > 0) then
				item_value:= abs(cost);
			end
		else if (tval in [55, 60, 65]) then
			begin { Wands rods, and staves }
			if (index(name, '|') > 0) then
				case tval of
					55:
						item_value:= 70;
					60:
						item_value:= 60;
					65:
						item_value:= 50;
					else
						;
					end
			else if (index(name, '^') = 0) then
				begin
				item_value:= cost + (cost div 20) * p1
				end
			end
		end
	end;


{ Asking price for an item }
//[psect(store$code)]
function sell_price (snum : integer; var max_sell,min_sell  : integer;
		item : treasure_type ) : integer;
	var
	i1 : integer;

	begin
{	if (snum = 7) then
		begin
		i1 := item.cost;
		max_sell := trunc(i1*(1+owners[store[7].owner].max_inflate));
		min_sell := trunc(i1*(1+owners[store[7].owner].min_inflate));
		if (min_sell > max_sell) then
			min_sell := max_sell;
		sell_price := i1
		end
	else}
		with store[snum] do
			begin
			i1 := item_value(item);
			if (item.cost > 0) then
				begin
				i1 := i1 + Trunc(i1*rgold_adj[owners[owner].owner_race,py.misc.prace]);
				if (i1 < 1) then
					i1 := 1;
				max_sell := Trunc(i1 * (1 + owners[owner].max_inflate));
				min_sell := Trunc(i1 * (1 + owners[owner].min_inflate));
				if (min_sell > max_sell) then
					min_sell := max_sell;
				sell_price := i1
				end
			else
				begin
				max_sell := 0;
				min_sell := 0;
				sell_price := 0
				end
			end
	end;


{ Check to see if he will be carrying too many objects }
//[psect(store$code)]
function store_check_num(store_num: integer): boolean;
	var
//	item_num,
	i1: integer;
//	flag: Boolean;

	begin
	store_check_num:= false;
	with store[store_num] do
		if (store_ctr < store_inven_max) then
			store_check_num:= true
		else if ((invenTemp.subval > 255)
		and (invenTemp.subval < 512)) then
			for i1:= 1 to store_ctr do
				with store_inven[i1].sitem do
					if (tval = invenTemp.tval) then
						if (subval = invenTemp.subval) then
							store_check_num:= True
	end;


{ Add the item in INVEN_MAX to store's inventory. }
{[psect(store$code)]}
procedure store_carry(store_num: Integer; var ipos: Integer);
	var
	item_num, item_val, typ, subt, icost, dummy: integer;
	flag: Boolean;

{ Insert INVEN_MAX at given location }
	procedure insert(store_num, pos, icost: integer);
		var
			i1: integer;
		begin
		with store[store_num] do
			begin
			for i1                 := store_ctr downto pos do
				store_inven[i1 + 1]:= store_inven[i1];
			store_inven[pos].sitem := invenTemp;
			store_inven[pos].scost := -icost;
			store_ctr              := store_ctr + 1
			end
		end;

{ Store_carry routine }
	begin
	ipos:= 0;
	identify(invenTemp);
	known2(invenTemp.name);
	sell_price(store_num, icost, dummy, invenTemp);
	if (icost > 0) then
		begin
		with invenTemp do
			with store[store_num] do
				begin
				item_val:= 0;
				item_num:= number;
				flag    := false;
				typ     := tval;
				subt    := subval;
				repeat
					item_val:= item_val + 1;
					with store_inven[item_val].sitem do
						if (typ = tval) then
							begin
							if (subt = subval) then { Adds to other item }
								if (subt > 255) then
									begin
									if (number < 24) then
										number:= number + item_num;
									flag      := true
									end
							end
						else if (typ > tval) then
							begin { Insert into list }
							insert(store_num, item_val, icost);
							flag:= true;
							ipos:= item_val
							end;
					until ((item_val >= store_ctr) or (flag));

				if (not(flag)) then { Becomes last item in list }
					begin
					insert(store_num, store_ctr + 1, icost);
					ipos:= store_ctr
					end
				end
		end
	end;



{ Destroy an item in the store's inventory.  Note that if 'one_of' is false,
  an entire slot is destroyed }
//[psect(store$code)]
procedure store_destroy(store_num, item_val: integer; one_of: boolean);
	var
	i2: integer;

	begin
	with store[store_num] do
		begin
		invenTemp:= store_inven[item_val].sitem;
		with store_inven[item_val].sitem do
			begin
			if ((number > 1) and (subval < 512) and (one_of)) then
				begin
				number                     := number - 1;
				invenTemp.number:= 1
				end
			else
				begin
				for i2:= item_val to store_ctr - 1 do
					store_inven[i2]:= store_inven[i2 + 1];
				store_inven[store_ctr].sitem:= blank_treasure;
				store_inven[store_ctr].scost:= 0;
				store_ctr                   := store_ctr - 1
				end
			end
		end
	end;



{ Initializes the stores with owners }
//[psect(setup$code)]
procedure store_init;
	var
	i1, i2, i3: integer;

	begin
	i1:= max_owners div MAX_STORES;
	for i2:= 1 to max_stores do
		with store[i2] do
			begin
			owner     := MAX_STORES * (randint(i1) - 1) + i2;
			insult_cur:= 0;
			store_open:= 0;
			store_ctr := 0;
			for i3    := 1 to store_inven_max do
				begin
				store_inven[i3].sitem:= blank_treasure;
				store_inven[i3].scost:= 0
				end
			end
	end;


{ Creates an item and inserts it into store's inven }
//[psect(store$code)]
procedure store_create(store_num: integer);
	var
	i1, tries, cur_pos, dummy: integer;

	begin
	tries:= 0;
	popt(cur_pos);
	with store[store_num] do
		repeat
			if (store_num = 7) then
				begin
				i1             := randint(blk_mkt_max);
				t_list[cur_pos]:= blk_mkt_init[i1];
				end
			else
				begin
				i1:= store_choice[store_num, randint(store_choices)];
				t_list[cur_pos]:= inventory_init[i1];
				magic_treasure(cur_pos, obj_town_level)
				end;
			invenTemp:= t_list[cur_pos];
			if (store_check_num(store_num)) then
				with t_list[cur_pos] do
					if (cost > 0) then { Item must be good }
						if (cost < owners[owner].max_cost) then
							begin
							store_carry(store_num, dummy);
							tries:= 10
							end;
			tries:= tries + 1;
			until (tries > 3);

//dengland Prevent this from being callled for every item in every store
	if store_num = 7 then
		do_black_market;

	pusht(cur_pos)
	end;


{ Initialize and up-keep the store's inventory. }
//[psect(store$code)]
procedure store_maint;
	var
	i1,
	i2: Integer;
	dummy: integer;

	begin
	for i1:= 1 to MAX_STORES do
		with store[i1] do
			begin
			insult_cur:= 0;
			if (store_ctr > store_max_inven) then
				for i2:= 1 to (store_ctr - store_max_inven + 2) do
					store_destroy(i1, randint(store_ctr), false)
			else if (store_ctr < store_min_inven) then
				begin
//dengland 		Ensure that certain items are always available
				case i1 of
					1:
						begin
						invenTemp:= inventory_init[1];
						store_carry(i1, dummy);
						invenTemp:= inventory_init[104];
						store_carry(i1, dummy);
						end;
					4:
						begin
						invenTemp:= inventory_init[71];
						store_carry(i1, dummy);
						invenTemp:= inventory_init[72];
						store_carry(i1, dummy);
						invenTemp:= inventory_init[73];
						store_carry(i1, dummy);
						invenTemp:= inventory_init[74];
						store_carry(i1, dummy);
						end;
					5:
						begin
						invenTemp:= inventory_init[58];
						store_carry(i1, dummy);
						invenTemp:= inventory_init[98];
						store_carry(i1, dummy);
						end;
					6:
						begin
						invenTemp:= inventory_init[67];
						store_carry(i1, dummy);
						invenTemp:= inventory_init[68];
						store_carry(i1, dummy);
						invenTemp:= inventory_init[69];
						store_carry(i1, dummy);
						invenTemp:= inventory_init[70];
						store_carry(i1, dummy);
						end;
					end;

				for i2:= 1 to (store_min_inven - store_ctr + 2) do
					store_create(i1)
				end
			else
				begin
				for i2:= 1 to (1 + randint(store_turn_around)) do
					store_destroy(i1, randint(store_ctr), true);
				for i2:= 1 to (1 + randint(store_turn_around)) do
					store_create(i1)
				end
			end
	end;

procedure StoreTabCallback(const AListView: TScrListView; const ATab: Integer;
		const AData: Pointer; var ALabel: AnsiString);
	begin
	if ATab = 1 then
		ALabel:= 'Services'
	else
		ALabel:= 'Items   ';
	end;

procedure StoreItemCallback(const AListView: TScrListView;
		const AScreen: TCustomSimpleScreen; const APosition: TScreenPos;
		const ATab: Integer; const APage: Integer; const AItem: Integer;
		const AData: Pointer);
	var
	i1,
	i2,
	itm: Integer;
	store_num: Integer;
	out_val1,
	out_val2: AnsiString;
	cs: AnsiString;
	cc: AnsiChar;

	begin
	store_num:= Integer(AData);
	i1:= AItem - (APage * 12);

	if ATab = 1 then
		begin
		end
	else
		begin
		itm:= AItem + 1;

		invenTemp:= store[store_num].store_inven[itm].sitem;
		with invenTemp do
			if ((subval > 255) and (subval < 512)) then
				number:= 1;
		objdes(out_val1, @invenTemp, True);

		out_val2:= AnsiString(AnsiChar(97 + i1) + ') ' + string(out_val1));

		if (itm mod 2) = 0 then
			cc:= AnsiChar(COLR_STOR_TBLBG1)
		else
			cc:= AnsiChar(COLR_STOR_TBLBG2);

//dengland  I'm assuming these are FG colours on BG black (0)
		cs:= AnsiChar(Ord(cc) + Ord(COLR_NMHHLT)) +
				AnsiChar(Ord(cc) + Ord(COLR_NMDLMT)) +
				AnsiChar(Ord(cc) + COLR_STOR_TBLFG1);

		prt(out_val2, APosition.Y, APosition.X);

		if (store[store_num].store_inven[itm].scost < 0) then
			begin
			i2:= Abs(store[store_num].store_inven[itm].scost);
			i2:= i2 + Trunc(i2 * chr_adj);
			out_val2:= PadLeft(AnsiString(IntToStr(i2)), 7);
			end
		else
			out_val2:= PadLeft(AnsiString(IntToStr(
					store[store_num].store_inven[itm].scost)), 7) + ' [Fixed]';

		prt(out_val2, APosition.Y, 60);

		PutColourCont(cs, 80, APosition.Y, APosition.X);
		end;
	end;


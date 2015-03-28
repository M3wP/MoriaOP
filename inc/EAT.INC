{ Eat some food.}
//[psect(misc2$code)]
procedure eat;
	var
	i1: unsigned;
	i2,
//	i3,
	item_val: Integer;
	t: TMoriaInvenType;
//	out_val: vtype;
	redraw,
	ident: Boolean;

	begin
	reset_flag:= true;
//	if (inven_ctr > 0) then
//		begin
//		if (find_range([80], i2, i3)) then
//			begin
//			redraw:= false;
//			if (get_item(item_val, 'Eat What?', redraw, i2, i3)) then
			if  GetItemFromInv(t, item_val, 'Eat what?',
					'You are not carrying any food.', redraw, [80]) then
//				with inventory[item_val] do
				with newInventory[t, item_val] do
					begin
					if (redraw) then
						draw_cave;
					reset_flag:= False;
					i1        := flags;
					ident     := True;
					while (i1 > 0) do
						begin
						i2:= bit_pos(i1);
{ Foods }
						case (i2) of
							1:
								with py.flags do
									begin
//dengland  Applying this level modifier here seems a tad forced.  Should instead
//		have different potion items/subvals to indicate concentration and name
//		them accordingly.  This should apply to all potions with level modifiers...
									poisoned:= poisoned + randint(10) + level;
									Opusii_vomit(35);
									ident:= true;
									end;
							2:
								with py.flags do
									begin
									blind:= blind + randint(250) + 10 *
											level + 100;
									draw_cave;
									msg_print(
										'A veil of darkness surrounds you.');
									ident:= true;
									end;
							3:
								with py.flags do
									begin
									afraid:= afraid + randint(10) + level;
									msg_print('You feel terrified!');
									ident:= true;
									end;
							4:
								with py.flags do
									begin
									confused:= confused + randint(10) + level;
									msg_print('You feel drugged.');
									end;
							5:
								with py.flags do
									image:= image + randint(200) + 25 *
											level + 200;
							6:
								ident:= cure_poison;
							7:
								ident:= cure_blindness;
							8:
								with py.flags do
									if (afraid > 1) then
										begin
										afraid:= 1;
										ident := true;
										end;
							9:
								ident:= cure_confusion;
							10:
								ident:= lose_str;
							11:
								ident:= lose_con;
							12:
								ident:= lose_int;
							13:
								ident:= lose_wis;
							14:
								ident:= lose_dex;
							15:
								ident:= lose_chr;
							16:
								with py.stat do
									if (str > cstr) then
										begin
										cstr:= str;
										msg_print(
											'You feel your strength returning.');
										prt_strength;
										prt_max_weight;
										ident:= true;
										end;
							17:
								with py.stat do
									if (con > ccon) then
										begin
										ccon:= con;
										msg_print(
											'You feel your health returning.');
										prt_constitution;
										prt_mhp;
										ident:= true;
										end;
							18:
								with py.stat do
									if (py.stat.int > cint) then
										begin
										cint:= py.stat.int;
										msg_print('Your head spins a moment.');
										prt_intelligence;
										ident:= true;
										end;
							19:
								with py.stat do
									if (wis > cwis) then
										begin
										cwis:= wis;
										msg_print(
											'You feel your wisdom returning.');
										prt_wisdom;
										ident:= true;
										end;
							20:
								with py.stat do
									if (dex > cdex) then
										begin
										cdex:= dex;
										msg_print('You feel more dextrous.');
										prt_dexterity;
										ident:= true;
										end;
							21:
								with py.stat do
									if (chr > cchr) then
										begin
										cchr:= chr;
										msg_print('Your skins starts itching.');
										prt_charisma;
										ident:= true;
										end;
							22:
								ident:= hp_player(randint(3),
									'poisonous food.');
							23:
								ident:= hp_player(randint(6),
									'poisonous food.');
							24:
								ident:= hp_player(randint(12),
									'poisonous food.');
							25:
								ident:= hp_player(damroll('3d6'),
									'poisonous food.');
							26:
								ident:= hp_player(damroll('3d12'),
									'poisonous food.');
							27:
								ident:= hp_player( -randint(4),
									'poisonous food.');
							28:
								ident:= hp_player( -randint(8),
									'poisonous food.');
							29:
								ident:= hp_player( -damroll('2d8'),
									'poisonous food.');
							30:
								ident:= hp_player( -damroll('3d8'),
									'poisonous food.');
							31:
								with py.misc do
									begin
									mhp:= mhp - 1;
									if (mhp < chp) then
										chp:= mhp;
									take_hit(1, 'poisonous food.');
									prt_mhp;
									prt_chp;
									ident:= true;
									end;
							else
								;
							end;
{ End of food actions. }
						end;

					if (ident) then
						identify(newInventory[t, item_val]);

					if (flags <> 0) then
						with py.misc do
							begin
							exp:= exp + round(level / lev);
							prt_experience;
							end;
					add_food(p1);
					py.flags.status:= ($FFFFFFFC and py.flags.status);
					prt_hunger;
					prt_cur_weight;

					DescRemain(t, item_val);
					InvenDestroy(t, item_val);
					end
			else if (redraw) then
				draw_cave;
//			end
//		else
//			msg_print('You are not carrying any food.');
//		end
//	else
//		msg_print('But you are not carrying anything.');
	end;

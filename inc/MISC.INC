{ Use date and time to produce random seed }
//[psect(setup$code)]
function get_seed: LongInt;
	begin
	Randomize;
	get_seed:= System.RandSeed;
	end;

//dengland  Wrap System.RandSeed
function  GetSeed: LongInt;
	begin
	Result:= System.RandSeed;
	end;

procedure SetSeed(AValue: LongInt);
	begin
	System.RandSeed:= AValue;
	end;


{ Computes current weight limit }
//[psect(misc4$code)]
function weight_limit: Integer;
	var
	weight_cap: Integer;

	begin
	weight_cap := py.stat.cstr * player_weight_cap + py.misc.wt;
	if (weight_cap > 3000) then
		weight_cap := 3000;
	weight_limit := weight_cap;
	end;


{ Generates a random integer x where 1<=X<=MAXVAL }
function randint(maxval: integer ): Integer;
	begin
	randint := Random(maxval) + 1
	end;


function rand_rep(num: Integer; die: Integer): Integer;
	var
	i : byte;

	begin
	Result:= num;
	for i := 1 to num do
		Inc(Result, random(die));
	end;


{ Generates a random integer number of NORMAL distribution }
//[psect(misc1$code)]
function randnor(mean, stand: Integer): Integer;
	begin
	randnor :=  Trunc(Sqrt(-2.0 * Ln(randint(9999999)/10000000.0))*
			Cos(6.283*(randint(9999999)/10000000.0))*stand) + mean;
	end;


{ Returns position of first set bit }
function bit_pos(var test: Unsigned): Integer;
	var
	bit: LongWord;
	f: Byte;

	begin
	bit := 1;
	for f := 1 to 32 do
		if (bit and test) = bit then
			begin
			test := test and (not bit);
			Exit(f)
			end
		else
			bit := bit shl 1;
	bit_pos:= 0
	end;


{ Checks a co-ordinate for in bounds status }
//[psect(misc1$code)]
function in_bounds(y, x: integer): Boolean;
	begin
	if  ((y > 1) and (y < cur_height - 1)
	and  (x > 1) and (x < cur_width - 1)) then
		in_bounds := True
	else
		in_bounds := False;
	end;


{ Distance between two points }
//dengland  Implemented as per BOSS (description in DISTANCE.MAR file)
function distance (y1, x1, y2, x2 : integer) : integer;
	var
	dx,
	dy,
	min_x_y: Integer;

	begin
	dy := abs(y1-y2);
	dx := abs(x1-x2);

	if dx < dy then
		min_x_y := dx
	else
		min_x_y := dy;

	distance := (2 * (dy + dx) - min_x_y) div 2
	end;


{ Checks points north, south, east, and west for a type }
//[psect(misc1$code)]
function next_to4 (y, x: Integer; group_set: Obj_Set): Integer;
	var
	i1 : Integer;

	begin
	i1 := 0;
	if (y > 1) then
		if (cave[y-1,x].fval in group_set) then
			i1 := i1 + 1;

	if (y < cur_height) then
		if (cave[y+1,x].fval in group_set) then
			i1 := i1 + 1;

	if (x > 1) then
		if (cave[y,x-1].fval in group_set) then
			i1 := i1 + 1;

	if (x < cur_width) then
		if (cave[y,x+1].fval in group_set) then
			i1 := i1 + 1;

	next_to4 := i1
	end;


{ Checks all adjacent spots for elements }
//[psect(misc1$code)]
function next_to8 (y, x: Integer; group_set: Obj_Set ): Integer;
	var
	i1,
	i2,
	i3: Integer;

	begin
	i1 := 0;
	for i2 := (y - 1) to (y + 1) do
		for i3 := (x - 1) to (x + 1) do
			if (in_bounds(i2,i3)) then
				if (cave[i2,i3].fval in group_set) then
					i1 := i1 + 1;

	next_to8 := i1
	end;


{ Link all free space in treasure list together }
//[psect(generate$code)]
procedure tlink;
	var
	i1 : Integer;

	begin
	for i1 := 1 to MAX_TALLOC do
		begin
		t_list[i1] := blank_treasure;
		t_list[i1].p1 := i1 - 1;
		end;

	tcptr := MAX_TALLOC;
	end;


{ Link all free space in monster list together }
//[psect(generate$code)]
procedure mlink;
	var
	i1 : Integer;

	begin
	for i1 := 1 to MAX_MALLOC do
		begin
		m_list[i1] := blank_monster;
		m_list[i1].nptr := i1 - 1;
		end;
	m_list[2].nptr := 0;
	muptr := 0;
	mfptr := MAX_MALLOC;
	end;


{ Initializes M_LEVEL array for use with PLACE_MONSTER }
//[psect(setup$code)]
procedure init_m_level;
	var
	i1,
	i2,
	i3: Integer;

	begin
	i1 := 1;
	i2 := 0;
	i3 := MAX_CREATURES - WIN_MON_TOT;
	repeat
		m_level[i2] := 0;
		while ((i1 <= i3) and (c_list[i1].level = i2)) do
			begin
			m_level[i2] := m_level[i2] + 1;
			i1 := i1 + 1;
			end;
		i2 := i2 + 1;
		until (i2 > MAX_MONS_LEVEL);

	for i1 := 2 to MAX_MONS_LEVEL do
		m_level[i1] := m_level[i1] + m_level[i1-1];
	end;


{ Initializes T_LEVEL array for use with PLACE_OBJECT }
//[psect(setup$code)]
procedure init_t_level;
	var
	i1,
	i2: Integer;

	begin
	i1 := 1;
	i2 := 0;
	repeat
		while (i1 <= MAX_OBJECTS) and (object_list[i1].level = i2) do
			begin
			t_level[i2] := t_level[i2] + 1;
			i1 := i1 + 1;
			end;
		i2 := i2 + 1;
		until (i2 > MAX_OBJ_LEVEL) or (i1 > MAX_OBJECTS);

	for i1 := 1 to MAX_OBJ_LEVEL do
		t_level[i1] := t_level[i1] + t_level[i1 - 1];
	end;

 { Adjust prices of objects }
procedure price_adjust;
	var
	i1: Integer;

	begin
	for i1 := 1 to MAX_OBJECTS do
		with object_list[i1] do
			cost := Trunc(cost * cost_adj + 0.99);

	for i1 := 1 to inven_init_max do
		with inventory_init[i1] do
			cost := Trunc(cost * cost_adj + 0.99);
	end;


{ Converts input string into a dice roll
  Normal input string will look like '2d6', '3d8'... ect. }
//[psect(misc1$code)]
function damroll(dice: DType): Integer;
	const
	DIE_MARKER: AnsiChar = 'd';

	var
	i: Byte;
//	j: Byte;
	num,
	die: ShortString;

	begin
	i := Pos(DIE_MARKER, dice);
//	for j:= 1 to Length(dice) do
//		if dice[j] = DIE_MARKER then
//			begin
//			i:= j;
//			Break;
//			end;

	num:= Copy(dice, 1, i - 1);
	die:= Copy(dice, i + 1, Length(dice) - i);

	damroll:= rand_rep(StrToInt(string(num)), StrToInt(string(die)));
	end;


{ Returns true if no obstructions between two given points }
//[psect(misc1$code)]
function los(y1, x1, y2, x2: Integer): boolean;
	var
	ty,
	tx,
	stepy,
	stepx,
	p1,
	p2: Integer;
	slp,
	tmp: Real;
	flag: Boolean;

	begin
	ty  := (y1 - y2);
	tx  := (x1 - x2);
	flag:= True;
	if ((ty <> 0) or (tx <> 0)) then
		begin
		if (ty < 0) then
			stepy:= -1
		else
			stepy:= 1;
		if (tx < 0) then
			stepx:= -1
		else
			stepx:= 1;
		if (ty = 0) then
			repeat
				x2  := x2 + stepx;
				flag:= cave[y2, x2].fopen;
				until ((x1 = x2) or (not(flag)))
		else if (tx = 0) then
			repeat
				y2  := y2 + stepy;
				flag:= cave[y2, x2].fopen;
				until ((y1 = y2) or (not(flag)))
		else if (abs(ty) > abs(tx)) then
			begin
			slp:= Abs(tx / ty) * stepx;
			tmp:= x2;
			repeat
				y2 := y2 + stepy;
				tmp:= tmp + slp;
				p1 := Round(tmp - 0.1);
				p2 := Round(tmp + 0.1);
				if (not((cave[y2, p1].fopen) or (cave[y2, p2].fopen))) then
					flag:= False;
				until ((y1 = y2) or (not(flag)))
			end
		else
			begin
			slp:= abs(ty / tx) * stepy;
			tmp:= y2;
			repeat
				x2 := x2 + stepx;
				tmp:= tmp + slp;
				p1 := Round(tmp - 0.1);
				p2 := Round(tmp + 0.1);
				if (not((cave[p1, x2].fopen) or (cave[p2, x2].fopen))) then
					flag:= False;
				until ((x1 = x2) or (not(flag)))
			end;
		end;

	los:= flag;
	end;

procedure get_mon_background(amon: Monster_Type; var colr: AnsiChar);
	begin
	if (c_list[amon.mptr].cdefense and $0400) > 0 then
		colr:= COLR_CREA_INNCNT
	else if ((amon.csleep and $8000) <> 0)
	or  (amon.stuned > 0)
	or  (amon.confused) then
		colr:= COLR_CREA_NORESP
	else
		colr:= COLR_CREA_HOSTLE;
	end;

procedure get_mon_foreground(amon: Monster_Type; var colr: AnsiChar);
	begin
//todo Check creature.cchar and subclass for colour

	if  amon.elite then
		colr:= AnsiChar(Ord(colr) + attrFGLightBlue)
	else
		colr:= AnsiChar(Ord(colr) + attrFGWhite);
	end;

procedure get_ply_colour(var colr: AnsiChar);
	begin
	if  (py.flags.afraid > 0)
	or  (py.flags.paralysis > 0)
	or  (py.flags.confused > 0) then
		colr:= COLR_PLYR_NORESP
	else
		colr:= COLR_PLYR_NORMAL;

//	Poisoned
	if  (py.flags.poisoned > 0) then
		colr:= AnsiChar(Ord(colr) + attrFGLightGreen)
//	Weak from hunger
	else if (py.flags.status and $00000002) > 0 then
		colr:= AnsiChar(Ord(colr) + attrFGLightRed)
//	Invunerable/Hero/Blessed
	else if (py.flags.invuln > 0)
	or (py.flags.hero > 0)
	or (py.flags.shero > 0)
	or (py.flags.blessed > 0) then
		colr:= AnsiChar(Ord(colr) + attrFGLightCyan)
//	Slow and not fast
	else if ((py.flags.status and $00000080) > 0)
	and ((py.flags.status and $00000040) = 0) then
		colr:= AnsiChar(Ord(colr) + attrFGYellow)
//	Fast and not slow
	else if ((py.flags.status and $00000040) > 0)
	and ((py.flags.status and $00000080) = 0) then
		colr:= AnsiChar(Ord(colr) + attrFGLightBlue)
	else
//	Normal
		colr:= AnsiChar(Ord(colr) + attrFGWhite);
	end;

procedure get_tre_colour(atre: Treasure_Type; afloor: Cave_Type; var colr: AnsiChar);
	begin
//todo More checks for treasure colours
	case atre.tval of
		100:
//			Money
			begin
//			Money can sometimes be in a wall.  Not sure about the less-than but
//			thats what other floor checking code does.
			if afloor.fval < 10 then
				colr:= AnsiChar(attrBGBlack)
			else
				colr:= AnsiChar(COLR_WALL_BKGRND);

			colr:= AnsiChar(Ord(colr) + attrFGYellow);
			end;
		102:
			if atre.subval = 99 then
//				Rune of warding
				colr:= COLR_TRES_RNWARD
			else
//				Visible trap
				colr:= COLR_TRES_VISTRP;
		103:
//			Rubble
			colr:= COLR_TRES_RUBBLE;
		104:
//			Open door
			colr:= COLR_TRES_OPNDOR;
		105:
//			Closed door
			colr:= COLR_TRES_CLSDOR;
		107:
//			Up stairs
			colr:= COLR_TRES_STAIRS;
		108:
//			Down stairs
			colr:= COLR_TRES_STAIRS;
		109:
//			Secret door
			colr:= COLR_WALL_NORMAL;
		110, 111:
//todo Use subval to give different colours for each of them?
//			Store entrance.  NB:  111 doesn't seem to be used?
			case atre.subval of
				101:
					colr:= AnsiChar(COLR_TRES_DORBKG + attrFGLightGreen);
				102:
					colr:= AnsiChar(COLR_TRES_DORBKG + attrFGLightMagenta);
				103:
					colr:= AnsiChar(COLR_TRES_DORBKG + attrFGWhite);
				104:
					colr:= AnsiChar(COLR_TRES_DORBKG + attrFGLightCyan);
				105:
					colr:= AnsiChar(COLR_TRES_DORBKG + attrFGLightBlue);
				106:
					colr:= AnsiChar(COLR_TRES_DORBKG + attrFGLightRed);
				107:
					colr:= AnsiChar(COLR_TRES_DORBKG + attrFGYellow);
				else
					colr:= AnsiChar(COLR_TRES_DORBKG + attrFGLightGray);
				end;
		else
			begin
//			OutputDebugString(PChar('Unhandled tval: ' + IntToStr(atre.tval)));
			colr:= COLR_NMHHLT;
			end;
		end;
	end;

{ Returns symbol for given row, column }
//[psect(misc5$code)]
procedure loc_symbol(y, x: Integer; var sym: AnsiChar; var colr: AnsiChar);
	begin
	colr:= COLR_NORMAL;

	with cave[y,x] do
		if ((cptr = 1) and (not(find_flag))) then
			begin
			sym := '@';
			get_ply_colour(colr);
			end
		else if (py.flags.blind > 0) then
			sym := ' '
		else
			begin
			if (cptr > 1) then
				begin
				with m_list[cptr] do
					if  ((ml)
					and  (((c_list[mptr].cmove and $00010000) = 0)
					or    (py.flags.see_inv))) then
						begin
						sym := c_list[mptr].cchar;
						get_mon_background(m_list[cptr], colr);
						get_mon_foreground(m_list[cptr], colr);
						end
					else if (tptr > 0) then
						begin
						get_tre_colour(t_list[tptr], cave[y, x], colr);
						sym := t_list[tptr].tchar;
						end
					else if (fval < 10) then
						sym := '.'
					else if fval = 11 then
						begin
						sym:= '%';
						colr:= COLR_WALL_DRKMGM;
						end
					else if fval = 12 then
						begin
						sym:= '%';
						colr:= COLR_WALL_QUARTZ;
						end
					else
						begin
						sym := '#';
						colr:= COLR_WALL_NORMAL;
						end;
				end
			else if (tptr > 0) then
				begin
				get_tre_colour(t_list[tptr], cave[y, x], colr);
				sym := t_list[tptr].tchar;
				end
			else if (fval < 10) then
				sym := '.'
			else if fval = 11 then
				begin
				sym:= '%';
				colr:= COLR_WALL_DRKMGM;
				end
			else if fval = 12 then
				begin
				sym:= '%';
				colr:= COLR_WALL_QUARTZ;
				end
			else
				begin
				sym := '#';
				colr:= COLR_WALL_NORMAL;
				end;
			end;
	end;


{ Tests a spot for light or field mark status }
//[psect(misc1$code)]
function test_light(y, x: Integer): Boolean;
	begin
	with cave[y,x] do
		if ((pl) or (fm) or (tl)) then
			test_light := True
		else
			test_light := False;
	end;


{ Prints the map of the dungeon }
//[psect(misc2$code)]
procedure prt_map;
	var
	i1,
	i2,
	i3,
	i4,
	i5,
	ypos,
	xpos,
	isp: Integer;

	floor_str: VType;
	color_str: AnsiString;

	tmp_char : AnsiChar;
	tmp_colr : AnsiChar;

	flag     : Boolean;

	begin
	screen.BeginUpdate;
	try
		xpos:= 0;

//dengland This is the only place that the global variable is used
//		redraw := false;  { Screen has been redrawn   }

		i3 := 1;          { Used for erasing dirty lines  }
		i4 := 14;         { Erasure starts in this column }
		for i1 := panel_row_min to panel_row_max do       { Top to bottom }
			begin
			i3 := i3 + 1;                 { Increment dirty line ctr }
			if (used_line[i3]) then       { If line is dirty... }
				begin
				erase_line(i3,i4);        { erase it. }
				used_line[i3] := false;   { Now it's a clean line }
				end;

			floor_str := ''; { Floor_str is string to be printed}
			color_str:= '';

			ypos := i1;      { Save row }
			flag := false;   { False until floor_str <> ''   }
			isp := 0;        { Number of blanks encountered  }
			for i2 := panel_col_min to panel_col_max do   { Left to right }
				begin
				tmp_colr:= AnsiChar(COLR_NORMAL);

				with cave[i1,i2] do
					begin        { Get character for location    }
					if (test_light(i1,i2)) then
						loc_symbol(i1, i2, tmp_char, tmp_colr)
					else if ((cptr = 1) and (not(find_flag))) then
						begin
						tmp_char := '@';
						get_ply_colour(tmp_colr);
						end
					else if (cptr > 1) then
						if (m_list[cptr].ml) then
							loc_symbol(i1, i2, tmp_char, tmp_colr)
						else
							tmp_char := ' '
					else
						tmp_char := ' ';

					if (tmp_char = ' ') then{ If blank... }
						begin
						if (flag) then      { If floor_str <> '' then }
							begin
							isp := isp + 1; { Increment blank ctr }
							if (isp > 3) then       { Too many blanks, print}
								begin                 { floor_str and reset }
								print(floor_str, color_str, ypos, xpos);
								flag := false;
								isp := 0;
								end;
							end
						end
					else
						begin
						if (flag) then      { Floor_str <> '' }
							begin
							if (isp > 0) then       { Add on the blanks }
								begin
								for i5 := 1 to isp do
									begin
									floor_str:= floor_str + ' ';
									color_str:= color_str + COLR_NORMAL;
									end;
								isp := 0;
								end;                  { Add on the character  }
							floor_str:= floor_str + tmp_char;
							color_str:= color_str + tmp_colr;
							end
						else
							begin             { Floor_str = '' }
							xpos := i2;     { Save column for printing }
							flag := true;   { Set flag to true }
							floor_str:= tmp_char;  { Floor_str <> '' }
							color_str:= tmp_colr;
							end;
						end;
					end;
				end;

			if (flag) then   { Print remainder, if any }
				print(floor_str, color_str, ypos, xpos);
			end;
		finally
		screen.EndUpdate;
		end;
	end;


{ Compact monsters }
//[psect(misc2$code)]
procedure compact_monsters;
	var
	i1,
	i2,
	i3,
//	ctr,
	cur_dis: Integer;
	delete_1,
	delete_any: Boolean;

	begin
	cur_dis := 66;
	delete_any := false;
	repeat
		i1 := muptr;
		i2 := 0;
		repeat
			delete_1 := false;
			i3 := m_list[i1].nptr;
			with m_list[i1] do
				if (cur_dis > cdis) then
					if (randint(3) = 1) then
						begin
						if (i2 = 0) then
							muptr := i3
						else
							m_list[i2].nptr := i3;

						cave[fy,fx].cptr := 0;
						m_list[i1] := blank_monster;
						m_list[i1].nptr := mfptr;
						mfptr := i1;
//						ctr := ctr + 1;
						delete_1 := True;
						delete_any := True;
						end;

			if (not(delete_1)) then
				i2 := i1;

			i1 := i3;
			until (i1 = 0);

		if (not(delete_any)) then
			cur_dis := cur_dis - 6;

		until (delete_any);

	if (cur_dis < 66) then
		prt_map;
	end;


{ Returns a pointer to next free space }
//[psect(misc3$code)]
procedure popm(var x: Integer);
	begin
	if (mfptr < 1) then
		compact_monsters;
	x := mfptr;
	mfptr := m_list[x].nptr;
	end;


{ Pushs a record back onto free space list }
//[psect(misc3$code)]
procedure pushm(x: Integer);
	begin
	m_list[x] := blank_monster;
	m_list[x].nptr := mfptr;
	mfptr := x;
	end;


{ Gives Max hit points }
//[psect(misc3$code)]
function max_hp(hp_str : dtype) : integer;
    var
	i : byte;

	begin
	i := pos('d', string(hp_str));
	max_hp:= strtoint(Copy(string(hp_str), 1, i - 1)) *
			strtoint(Copy(string(hp_str), i + 1, Length(hp_str) - i));
	end;


{ Places a monster at given location }
//[psect(misc3$code)]
procedure place_monster(y, x, z: Integer; slp: Boolean; const AElite: Boolean = False);
	var
//	i1,
	cur_pos: Integer;

	begin
	popm(cur_pos);
	with m_list[cur_pos] do
		begin
		fy := y;
		fx := x;
		mptr := z;
		nptr := muptr;
		muptr := cur_pos;
		if ((c_list[z].cdefense and $4000) <> 0) then
			hp := max_hp(c_list[z].hd)
		else
			hp := damroll(c_list[z].hd);
		cspeed := c_list[z].speed + py.flags.speed;
		stuned := 0;
		cdis := distance(char_row,char_col,y,x);
		cave[y,x].cptr := cur_pos;
		if (slp) then
			begin
			csleep := (c_list[z].sleep div 5) + randint(c_list[z].sleep)
			end
		else
			csleep := 0;

		elite:= AElite;
		end
	end;


{ Places a monster at given location }
//[psect(misc3$code)]
procedure place_win_monster;
	var
	cur_pos, y, x: Integer;

	begin
	if (not(total_winner)) then
		begin
		popm(cur_pos);
		with m_list[cur_pos] do
			begin
			repeat
				y := randint(cur_height-2)+1;
				x := randint(cur_width-2)+1;
				until ((cave[y,x].fval in [1,2,4])
					and (cave[y,x].cptr = 0)
					and (cave[y,x].tptr = 0)
					and (distance(y,x,char_row,char_col) > max_sight));
			fy := y;
			fx := x;
			mptr := randint(win_mon_tot) +
					m_level[max_mons_level] + m_level[0];
			nptr := muptr;
			muptr := cur_pos;
			if ((c_list[mptr].cdefense and $4000) <> 0) then
				hp := max_hp(c_list[mptr].hd)
			else
				hp := damroll(c_list[mptr].hd);
			cspeed := c_list[mptr].speed + py.flags.speed;
			stuned := 0;
			cdis := distance(char_row,char_col,y,x);
			cave[y,x].cptr := cur_pos;
			csleep := 0;
			end;
		end;
	end;


{ Allocates a random monster }
//[psect(misc3$code)]
procedure alloc_monster(alloc_set: Obj_Set; num, dis: Integer; slp: Boolean );
	var
	y,
	x,
//	a,
//	b,
	i1,
	i2,
	i3: Integer;
	e: Boolean;

	begin
	for i1 := 1 to num do
		begin
		e:= False;

		repeat
			y := randint(cur_height-2)+1;
			x := randint(cur_width-2)+1;
			until ((cave[y,x].fval in alloc_set)
				and (cave[y,x].cptr = 0)
				and (cave[y,x].fopen)
				and (distance(y,x,char_row,char_col) > dis));

		if (dun_level = 0) then
			i2 := randint(m_level[0])
		else if (dun_level > max_mons_level) then
			i2 := randint(m_level[max_mons_level]) + m_level[0]
		else if (randint(mon_nasty) = 1) then
			begin
			e:= True;
			i2 := dun_level + abs(randnor(0,4)) + 1;
			if (i2 > max_mons_level) then
				i2 := max_mons_level;
			i3 := m_level[i2] - m_level[i2-1];
			i2 := randint(i3) + m_level[i2-1]
			end
		else
			i2 := randint(m_level[dun_level]) + m_level[0];

		place_monster(y,x,i2,slp, e);
		end
	end;


{ Places creature adjacent to given location }
//[psect(misc3$code)]
function summon_monster(var y, x: Integer; slp: Boolean): Boolean;
	var
	i1,
	i2,
	i3,
	i4,
	i5: Integer;

	begin
	i1 := 0;
	i5 := dun_level + MON_SUMMON_ADJ;
	summon_monster := False;
	if (i5 > max_mons_level) then
		i4 := max_mons_level
	else
		i4 := i5;
	if (dun_level = 0) then
		i4 := randint(m_level[0])
	else
		i4 := randint(m_level[i4]) + m_level[0];
	repeat
		i2 := y - 2 + randint(3);
		i3 := x - 2 + randint(3);
		if (in_bounds(i2,i3)) then
			with cave[i2,i3] do
				if (fval in [1,2,4,5]) then
					if (cptr = 0) then
						if (fopen) then
							begin
							place_monster(i2,i3,i4,slp);
							summon_monster := true;
							i1 := 9;
							y := i2;
							x := i3;
							end;
		i1 := i1 + 1;
		until (i1 > 9);
	end;


{ Places undead adjacent to given location }
//[psect(misc3$code)]
function summon_undead(var y, x: Integer): Boolean;
	var
	i1,
	i2,
	i3,
	i4,
	i5,
	ctr: Integer;

	begin
	i1 := 0;
	summon_undead := False;
	i4 := m_level[max_mons_level] + m_level[0];
	repeat
		i5 := randint(i4);
		ctr := 0;
		repeat
			if ((c_list[i5].cdefense and $0008) <> 0) then
				begin
				ctr := 20;
				i4  := 0;
				end
			else
				begin
				i5 := i5 + 1;
				if (i5 > i4) then
					ctr := 20
				else
					ctr := ctr + 1;
				end;
			until(ctr > 19)
		until(i4 = 0);

	repeat
		i2 := y - 2 + randint(3);
		i3 := x - 2 + randint(3);
		if (in_bounds(i2,i3)) then
			with cave[i2,i3] do
				if (fval in [1,2,4,5]) then
					if ((cptr = 0) and (fopen)) then
						begin
						place_monster(i2,i3,i5,false);
						summon_undead := True;
						i1 := 9;
						y := i2;
						x := i3;
						end;
		i1 := i1 + 1;
		until (i1 > 9);
	end;


{ If too many objects on floor level, delete some of them   }
//[psect(misc2$code)]
procedure compact_objects;
	var
	i1,
	i2,
	ctr,
	cur_dis: Integer;
	flag: Boolean;

	begin
	ctr := 0;
	cur_dis := 66;
	repeat
		for i1 := 1 to cur_height do
			for i2 := 1 to cur_width do
				with cave[i1,i2] do
					if (tptr > 0) then
						if (distance(i1,i2,char_row,char_col) > cur_dis) then
							begin
							flag := False;
							with t_list[tptr] do
								case tval of
									102:
										if (subval in [1,6,9]) then
											flag := True
										else if (randint(4) = 1) then
											flag := True;
									103:
										Flag := True;
									104, 105:
										if (randint(4) = 1) then
											flag := True;
									107, 108:
										;
									else
										if (randint(8) = 1) then
											flag := True;
									end;
							if (flag) then
								begin
								fopen := True;
								t_list[tptr] := blank_treasure;
								t_list[tptr].p1 := tcptr;
								tcptr := tptr;
								tptr := 0;
								ctr := ctr + 1;
								end;
							end;
		if (ctr = 0) then
			cur_dis := cur_dis - 6;
		until (ctr > 0);

	if (cur_dis < 66) then
		prt_map;
	end;


{ Gives pointer to next free space  }
//[psect(misc4$code)]
procedure popt(var x : integer);
//	var
//	i1 : integer;

	begin
	if (tcptr < 1) then
		compact_objects;
	x := tcptr;
	tcptr := t_list[x].p1;
	end;


{ Pushs a record back onto free space list  }
//[psect(misc4$code)]
procedure pusht(x : integer);
	begin
	t_list[x] := blank_treasure;
	t_list[x].p1 := tcptr;
	tcptr := x;
	end;


{ Order the treasure list by level  }
//[psect(setup$code)]
procedure sort_objects;
	var
	i1,
	i2,
	i3,
	gap: Integer;
	tmp: Treasure_Type;

	begin
	gap := max_objects div 2;
	while (gap > 0) do
		begin
		for i1 := gap + 1 to MAX_OBJECTS do
			begin
			i2 := i1 - gap;
			while (i2 > 0) do
				begin
				i3 := i2 + gap;
				if (object_list[i2].level > object_list[i3].level) then
					begin
					tmp := object_list[i2];
					object_list[i2] := object_list[i3];
					object_list[i3] := tmp;
					end
				else
					i2 := 0;
				i2 := i2 - gap;
				end;
			end;
		gap := gap div 2;
		end;
	end;



{ Chance of treasure having magic abilities
  Chance increases with each dungeon level  }
//[psect(misc4$code)]
procedure magic_treasure(x, level: Integer);
    var
	chance,
	special,
	cursed,
	i1: Integer;

{ Boolean : is object enchanted }
	function magik(chance: Integer) : Boolean;
		begin
		if (randint(100) <= chance) then
			magik := true
		else
			magik := false;
		end;

{ Enchant a bonus based on degree desired   }
	function m_bonus(base, max_std, level: Integer): Integer;
		var
		x,
		stand_dev: Integer;

		begin
		stand_dev:= Trunc(OBJ_STD_ADJ * level) + OBJ_STD_MIN;

		if (stand_dev > MAX_STD) then
			stand_dev := MAX_STD;

		x := Trunc(Abs(randnor(0, stand_dev)) / 10.0) + base;
		if (x < base) then
			m_bonus := base
		else
			m_bonus := x;
		end;

	begin
	chance := OBJ_BASE_MAGIC + level;
	if (chance > OBJ_BASE_MAX) then
		chance := OBJ_BASE_MAX;
	special := (chance div OBJ_DIV_SPECIAL);
	cursed  := Trunc(chance / OBJ_DIV_CURSED);
	with t_list[x] do
{ Depending on treasure type, it can have certain magical properties}
		case tval of
			34, 35, 36:
				begin { Armor and shields}
				if (magik(chance)) then
					begin
					toac := m_bonus(1,30,level);
					if (magik(special)) then
						case randint(9) of
							1:
								begin
								flags := flags or $02380000;
								name := name + ' (R)';
								toac := toac + 5;
								cost := cost + 2500;
								end;
							2:
								begin   { Resist Acid   }
								flags := flags or $00100000;
								name := name + ' (RA)';
								cost := cost + 1000;
								end;
							3, 4:
								begin   { Resist Fire   }
								flags := flags or $00080000;
								name := name + ' (RF)';
								cost := cost + 600;
								end;
							5, 6:
								begin   { Resist Cold   }
								flags := flags or $00200000;
								name := name + ' (RC)';
								cost := cost + 600;
								end;
							7, 8, 9:
								begin   { Resist Lightning}
								flags := flags or $02000000;
								name := name + ' (RL)';
								cost := cost + 500;
								end;
							end
					end
				else if (magik(cursed)) then
					begin
					toac := -m_bonus(1,40,level);
					cost := 0;
					flags := $80000000 or flags;
					end
				end;
			21, 22, 23:
				begin { Weapons   }
				if (magik(chance)) then
					begin
					tohit := m_bonus(0,40,level);
					todam := m_bonus(0,40,level);

//dengland			Don't allow both as 0.
					if  (tohit = 0)
					and (todam = 0) then
						todam:= 1;

					if (magik(special)) then
						case randint(16) of
							1:
								begin   { Holy Avenger  }
								flags := flags or $01418001;
								tohit := tohit + 5;
								todam := todam + 5;
								toac  := randint(4);
								p1    := 1 + randint(4);
								name  := name + ' (HA)';
								cost  := cost + p1*500;
								cost  := cost + 10000;
								end;
							2:
								begin   { Defender  }
								flags := flags or $07B80900;
								tohit := tohit + 3;
								todam := todam + 3;
								toac  := 5 + randint(5);
								name  := name + ' [%P4] (DF)';
								p1    := randint(3);
								cost  := cost + p1*500;
								cost  := cost + 7500;
								end;
							3, 4:
								begin   { Slay Monster  }
								flags := flags or $01004000;
								tohit := tohit + 3;
								todam := todam + 3;
								name := name + ' (SM)';
								cost := cost + 5000;
								end;
							5, 6:
								begin   { Slay Dragon   }
								flags := flags or $00002000;
								tohit := tohit + 3;
								todam := todam + 3;
								name := name + ' (SD)';
								cost := cost + 4000;
								end;
							7, 8:
								begin   { Slay Evil }
								flags := flags or $00008000;
								tohit := tohit + 3;
								todam := todam + 3;
								name := name + ' (SE)';
								cost := cost + 4000;
								end;
							9, 10:
								begin   { Slay Undead   }
								flags := flags or $00010000;
								tohit := tohit + 2;
								todam := todam + 2;
								name := name + ' (SU)';
								cost := cost + 3000;
								end;
							11, 12, 13:
								begin   { Flame Tongue  }
								flags := flags or $00040000;
								tohit := tohit + 1;
								todam := todam + 3;
								name := name + ' (FT)';
								cost := cost + 2000;
								end;
							14, 15, 16:
								begin   { Frost Brand   }
								flags := flags or $00020000;
								tohit := tohit + 1;
								todam := todam + 1;
								name := name + ' (FB)';
								cost := cost + 1200;
								end;
							end;
					end
				else if (magik(cursed)) then
					begin
					tohit := -m_bonus(1,55,level);
					todam := -m_bonus(1,55,level);
					flags := $80000000 or flags;
					cost := 0;
					end
				end;
			20 :
				begin { Bows, crossbows, and slings   }
				if (magik(chance)) then
					tohit := m_bonus(1,30,level)
				else if (magik(cursed)) then
					begin
					tohit := -m_bonus(1,50,level);
					flags := $80000000 or flags;
					cost := 0;
					end;
				end;
			25 :
				begin { Digging tools }
				if (magik(chance)) then
					case randint(3) of
						1, 2:
							begin
							p1 := m_bonus(2,25,level);
							cost := cost + p1*100;
							end;
						3:
							begin
							p1 := -m_bonus(1,30,level);
							cost := 0;
							flags := $80000000 or flags;
							end;
						end;
				end;
			31 :
				begin { Gloves and Gauntlets  }
				if (magik(chance)) then
					begin
					toac := m_bonus(1,20,level);
					if (magik(special)) then
						case randint(2) of
							1  :
								begin
								flags := $00800000 or flags;
								name := name + ' of Free Action';
								cost := cost + 1000;
								end;
							2  :
								begin
								tohit := 1 + randint(3);
								todam := 1 + randint(3);
								name := name + ' of Slaying';
								cost := cost + (tohit+todam)*250;
								end;
							end;
					end
				else if (magik(cursed)) then
					begin
					if (magik(special)) then
						case randint(2) of
							1 :
								begin
								flags := $80000002 or flags;
								name := name + ' of Clumsiness';
								p1 := 1;
								end;
							2 :
								begin
								flags := $80000001 or flags;
								name := name + ' of Weakness';
								p1 := 1;
								end;
							end;
					toac := -m_bonus(1,40,level);
					p1   := -m_bonus(1,10,level);
					flags := $80000000 or flags;
					cost := 0;
					end
				end;
			30:
				begin { Boots }
				if (magik(chance)) then
					begin
					toac := m_bonus(1,20,level);
					if (magik(special)) then
						case randint(12) of
							1 :
								begin
								flags := $00001000 or flags;
								name := name + ' of Speed';
								p1 := 1;
								cost := cost + 5000;
								end;
							2, 3, 4, 5 :
								begin
								flags := $00000100 or flags;
								name := name + ' of Stealth';
								cost := cost + 500;
								end;
							else
								begin
								flags := $04000000 or flags;
								name := name + ' of Slow decent';
								cost := cost + 250;
								end
							end
					end
				else if (magik(cursed)) then
					begin
					case randint(3) of
						1 :
							begin
							flags := $80001000 or flags;
							name := name + ' of Slowness';
							p1 := -1;
							end;
						2 :
							begin
							flags := $80000200 or flags;
							name := name + ' of Noise';
							end;
						3 :
							begin
							flags := $80000000 or flags;
							name := name + ' of Great Mass';
							weight := weight * 5;
							end;
						end;
					cost := 0;
					ac := -m_bonus(2,45,level);
					end;
				end;
			33:
				begin { Helms }
				if (magik(chance)) then
					begin
					toac := m_bonus(1,20,level);
					if (magik(special)) then
						case subval of
							1, 2, 3, 4, 5  :
								case randint(3) of
									1 :
										begin
										p1 := randint(2);
										flags := $00000008 or flags;
										name := name + ' of Intelligence';
										cost := cost + p1*500;
										end;
									2 :
										begin
										p1 := randint(2);
										flags := $00000010 or flags;
										name := name + ' of Wisdom';
										cost := cost + p1*500;
										end;
									3 :
										begin
//										dengland I'm changing all infra-vision to
//										double size
//										p1 := 1 + randint(4);
										p1 := 4 + randint(4);
										flags := $40000000 or flags;
										name := name + ' of Infra-Vision';
//										cost := cost + p1*250;
										cost := cost + p1*125;
										end;
									end;
							6, 7, 8      :
								case randint(6) of
									1 :
										begin
										p1 := randint(3);
										flags := $00800007 or flags;
										name := name + ' of Might';
										cost := cost + 1000 + p1*500;
										end;
									2 :
										begin
										p1 := randint(3);
										flags := $00000030 or flags;
										name := name + ' of Lordliness';
										cost := cost + 1000 + p1*500;
										end;
									3 :
										begin
										p1 := randint(3);
										flags := $01380008 or flags;
										name := name + ' of the Magi';
										cost := cost + 3000 + p1*500;
										end;
									4 :
										begin
										p1 := randint(3);
										flags := $00000020 or flags;
										name := name + ' of Beauty';
										cost := cost + 750;
										end;
									5 :
										begin
										p1 := 1 + randint(4);
										flags := $01000040 or flags;
										name := name + ' of Seeing';
										cost := cost + 1000 + p1*100;
										end;
									6 :
										begin
										flags := $00000800 or flags;
										name := name + ' of Regeneration';
										cost := cost + 1500;
										end;
									end;
							end;
					end
				else if (magik(cursed)) then
					begin
					toac := -m_bonus(1,45,level);
					flags := $80000000 or flags;
					cost := 0;
					if (magik(special)) then
						case randint(7) of
							1 :
								begin
								p1 := -1;
								flags := $00000008 or flags;
								name := name + ' of Stupidity';
								end;
							2 :
								begin
								p1 := -1;
								flags := $00000010 or flags;
								name := name + ' of Dullness';
								end;
							3 :
								begin
								flags := $08000000 or flags;
								name := name + ' of Blindness';
								end;
							4 :
								begin
								flags := $10000000 or flags;
								name := name + ' of Timidness';
								end;
							5 :
								begin
								p1 := -1;
								flags := $00000001 or flags;
								name := name + ' of Weakness';
								end;
							6 :
								begin
								flags := $00000400 or flags;
								name := name + ' of Teleportation';
								end;
							7 :
								begin
								p1 := -1;
								flags := $00000020 or flags;
								name := name + ' of Ugliness';
								end;
							end;
					p1 := p1*randint(5);
					end;
				end;
			45:
				begin { Rings }
				case subval of
					1,2,3,4,5,6 :
						if (magik(cursed)) then
							begin
							p1 := -m_bonus(1,20,level);
							flags := $80000000 or flags;
							cost := -cost;
							end
						else
							begin
							p1 := m_bonus(1,10,level);
							cost := cost + p1*100;
							end;
					7 :
						if (magik(cursed)) then
							begin
							p1 := -randint(3);
							flags := $80000000 or flags;
							cost := -cost;
							end
						else
							p1 := 1;
					8  :
						begin
						p1 := 5 * m_bonus(1,20,level);
						cost := cost + p1*100;
						end;
					22 :
						begin   { Increase damage   }
						todam := m_bonus(1,20,level);
						cost := cost + todam*100;
						if (magik(cursed)) then
							begin
							todam := -todam;
							flags := $80000000 or flags;
							cost := -cost;
							end
						end;
					23 :
						begin   { Increase To-Hit   }
						tohit := m_bonus(1,20,level);
						cost := cost + todam*100;
						if (magik(cursed)) then
							begin
							tohit := -tohit;
							flags := $80000000 or flags;
							cost := -cost;
							end
						end;
					24 :
						begin   { Protection    }
						toac := m_bonus(1,20,level);
						cost := cost + todam*100;
						if (magik(cursed)) then
							begin
							toac := -toac;
							flags := $80000000 or flags;
							cost := -cost;
							end
						end;
					33 :
						begin   { Slaying   }
						todam := m_bonus(1,25,level);
						tohit := m_bonus(1,25,level);
						cost := cost + (tohit+todam)*100;
						if (magik(cursed)) then
							begin
							tohit := -tohit;
							todam := -todam;
							flags := $80000000 or flags;
							cost := -cost;
							end
						end;
					else
						;
					end;
				end;
			40:
				begin { Amulets   }
				case subval of
					1,2,3,4,5,6 :
						if (magik(cursed)) then
							begin
							p1 := -m_bonus(1,20,level);
							flags := $80000000 or flags;
							cost := -cost;
							end
						else
							begin
							p1 := m_bonus(1,10,level);
							cost := cost + p1*100;
							end;
					7  :
						begin
						p1 := 5*m_bonus(1,25,level);
						if (magik(cursed)) then
							begin
							p1 := -p1;
							cost := -cost;
							flags := $80000000 or flags;
							end
						else
							cost := cost + 100*p1;
						end;
					else
						;
					end;
				end;
{ Subval should be even for store, odd for dungeon
  Dungeon found ones will be partially charged    }
			15:
				begin { Lamps and torches}
				if ((subval mod 2) = 1) then
					p1 := randint(p1);
				end;
			65:
				begin { Wands }
				case subval of
					1:
						p1 := randint(10) + 6;
					2:
						p1 := randint(8)  + 6;
					3:
						p1 := randint(5)  + 6;
					4:
						p1 := randint(8)  + 6;
					5:
						p1 := randint(4)  + 3;
					6:
						p1 := randint(8)  + 6;
					7:
						p1 := randint(20) + 12;
					8:
						p1 := randint(20) + 12;
					9:
						p1 := randint(10) + 6;
					10:
						p1 := randint(12) + 6;
					11:
						p1 := randint(10) + 12;
					12:
						p1 := randint(3)  + 3;
					13:
						p1 := randint(8)  + 6;
					14:
						p1 := randint(10) + 6;
					15:
						p1 := randint(5)  + 3;
					16:
						p1 := randint(5)  + 3;
					17:
						p1 := randint(5)  + 6;
					18:
						p1 := randint(5)  + 4;
					19:
						p1 := randint(8)  + 4;
					20:
						p1 := randint(8)  + 4;
					21:
						p1 := randint(8)  + 4;
					22:
						p1 := randint(24) + 12;
					23:
						p1 := randint(8)  + 4;
					24:
						p1 := randint(10) + 6;
					else
						;
                    end
				end;
			55:
				begin { Staves    }
				case subval of
					1:
						p1 := randint(20) + 12;
					2:
						p1 := randint(8)  + 6;
					3:
						p1 := randint(5)  + 6;
					4:
						p1 := randint(20) + 12;
					5:
						p1 := randint(15) + 6;
					6:
						p1 := randint(4)  + 5;
					7:
						p1 := randint(5)  + 3;
					8:
						p1 := randint(3)  + 1;
					9:
						p1 := randint(3)  + 1;
					10:
						p1 := randint(3)  + 1;
					11:
						p1 := randint(5)  + 6;
					12:
						p1 := randint(10) + 12;
					13:
						p1 := randint(5)  + 6;
					14:
						p1 := randint(5)  + 6;
					15:
						p1 := randint(5)  + 6;
					16:
						p1 := randint(10) + 12;
					17:
						p1 := randint(3)  + 4;
					18:
						p1 := randint(5)  + 6;
					19:
						p1 := randint(5)  + 6;
					20:
						p1 := randint(3)  + 4;
					21:
						p1 := randint(10) + 12;
					22:
						p1 := randint(3)  + 4;
					23:
						p1 := randint(3)  + 4;
					24:
						p1 := randint(3)  + 1;
					25:
						p1 := randint(10) + 6;
					else
						;
					end
				end;
			32:
				begin { Cloaks    }
				if (magik(chance)) then
					begin
					if (magik(special)) then
						case randint(2) of
							1:
								begin
								name := name + ' of Protection';
								toac := m_bonus(2,40,level);
								cost := cost + 250 + toac*100;
								end;
							2:
								begin
								toac := m_bonus(1,20,level);
								p1 := randint(3);
								flags := $00000100 or flags;
								name := name + ' of Stealth (%P1)';
								cost := cost + p1*500 + toac*100;
								end;
							end
					else
						begin
						toac := m_bonus(1,20,level);
						cost := cost + toac+100;
						end;
					end
				else if (magik(cursed)) then
					case randint(3) of
						1 :
							begin
							flags := $80000200 or flags;
							name := name + ' of Irritation';
							ac   :=  0;
							toac  := -m_bonus(1,10,level);
							tohit := -m_bonus(1,10,level);
							todam := -m_bonus(1,10,level);
							cost :=  0;
							end;
						2 :
							begin
							flags := $80000000 or flags;
							name := name + ' of Vulnerability';
							ac   := 0;
							toac := -m_bonus(10,100,level+50);
							cost := 0;
							end;
						3 :
							begin
							flags := $80000000 or flags;
							name := name + ' of Enveloping';
							toac  := -m_bonus(1,10,level);
							tohit := -m_bonus(2,40,level+10);
							todam := -m_bonus(2,40,level+10);
							cost := 0;
							end;
						end;
				end;
			2:
				begin { Chests    }
				case (randint(level)+4) of
					1  :
						begin
						name := name + '^ (Empty)';
						end;
					2  :
						begin
						flags := $00000001 or flags;
						name := name + '^ (Locked)';
						end;
					3, 4:
						begin
						flags := $00000011 or flags;
						name := name + '^ (Poison Needle)';
						end;
					5, 6:
						begin
						flags := $00000021 or flags;
						name := name + '^ (Poison Needle)';
						end;
					7,8,9:
						begin
						flags := $00000041 or flags;
						name := name + '^ (Gas Trap)';
						end;
					10,11:
						begin
						flags := $00000081 or flags;
						name := name + '^ (Explosion Device)';
						end;
					12,13,14  :
						begin
						flags := $00000101 or flags;
						name := name + '^ (Summoning Runes)';
						end;
					15,16,17  :
						begin
						flags := $00000071 or flags;
						name := name + '^ (Multiple Traps)';
						end;
					else
						begin
						flags := $00000181 or flags;
						name := name + '^ (Multiple Traps)';
						end;
					end;
				end;
			10,11,12,13:
				begin      { Arrows, bolts, ammo, and spikes   }
				if (tval in [11,12]) then
					if (magik(chance)) then
						begin
						tohit := m_bonus(1,35,level);
						todam := m_bonus(1,35,level);
						if (magik(special)) then
							case tval of  {CASE 1}
								11,12 :
									case randint(10) of   {CASE 2}
										1,2,3 :
											begin
											name := name + ' of Slaying';
											tohit := tohit + 5;
											todam := todam + 5;
											cost := cost + 20;
											end;
										4,5   :
											begin
											flags := flags or $00040000;
											tohit := tohit + 2;
											todam := todam + 4;
											name := name + ' of Fire';
											cost := cost + 25;
											end;
										6,7   :
											begin
											flags := flags or $00008000;
											tohit := tohit + 3;
											todam := todam + 3;
											name := name + ' of Slay Evil';
											cost := cost + 25;
											end;
										8,9   :
											begin
											flags := flags or $01004000;
											tohit := tohit + 2;
											todam := todam + 2;
											name := name + ' of Slay Monster';
											cost := cost + 30;
											end;
										10    :
											begin
											flags := flags or 00002000;
											tohit := tohit + 10;
											todam := todam + 10;
											name := name + ' of Dragon Slaying';
											cost := cost + 35;
											end;
										end; {CASE 2}
								else
									;
								end;  {CASE 1}
						end
					else if (magik(cursed)) then
						begin
						tohit := -m_bonus(5,55,level);
						todam := -m_bonus(5,55,level);
						flags := $80000000 or flags;
						cost := 0;
						end;

				number := 0;
				for i1 := 1 to 7 do
					number := number + randint(6);
				missle_ctr := missle_ctr + 1;
				if (missle_ctr > 65534) then
					missle_ctr := 1;
				subval := missle_ctr + 512;
				end;
			else
				;
			end
	end;

{places a trap at a given location}
//[psect(misc4$code)]
procedure place_trap(y, x, typ, subval: Integer);
	var
	cur_pos: Integer;
	cur_trap: Treasure_Type;

	begin
	if (typ = 1) then
		cur_trap := trap_lista[subval]
	else if (typ = 2) then
		cur_trap := trap_listb[subval]
	else
		cur_trap := blk_mkt_trap; {typ=3, one per level - mwk}
	popt(cur_pos);
	cave[y,x].tptr  := cur_pos;
	t_list[cur_pos] := cur_trap;
	end;


{ Places rubble at location y,x }
//[psect(misc4$code)]
procedure place_rubble(y, x: Integer);
	var
	cur_pos: Integer;

	begin
	popt(cur_pos);
	with cave[y,x] do
		begin
		tptr := cur_pos;
		fopen := False;
		end;
	t_list[cur_pos] := rubble;
	end;


//[psect(misc4$code)]
procedure place_open_door(y, x: Integer);
	var
	cur_pos: Integer;

	begin
	popt(cur_pos);
	with cave[y,x] do
		begin
		tptr := cur_pos;
		t_list[cur_pos] := door_list[1];
		fval  := corr_floor3.ftval;
		fopen := True;
		end;
	end;


//[psect(misc4$code)]
procedure place_broken_door(y, x: Integer);
	var
	cur_pos: Integer;

	begin
	popt(cur_pos);
	with cave[y,x] do
		begin
		tptr := cur_pos;
		t_list[cur_pos] := door_list[1];
		fval  := corr_floor3.ftval;
		fopen := True;
		t_list[cur_pos].p1 := 1;
		end;
	end;


//[psect(misc4$code)]
procedure place_closed_door(y, x: Integer);
	var
	cur_pos: Integer;

	begin
	popt(cur_pos);
	with cave[y,x] do
		begin
		tptr := cur_pos;
		t_list[cur_pos] := door_list[2];
		fval  := corr_floor3.ftval;
		fopen := false;
		end;
	end;


//[psect(misc4$code)]
procedure place_locked_door(y, x: Integer);
	var
	cur_pos: Integer;

	begin
	popt(cur_pos);
	with cave[y,x] do
		begin
		tptr := cur_pos;
		t_list[cur_pos] := door_list[2];
		fval  := corr_floor3.ftval;
		fopen := False;
		t_list[cur_pos].p1 := randint(10) + 10;
		end;
	end;


//[psect(misc4$code)]
procedure place_stuck_door(y, x: Integer);
	var
	cur_pos: Integer;

	begin
	popt(cur_pos);
	with cave[y,x] do
		begin
		tptr := cur_pos;
		t_list[cur_pos] := door_list[2];
		fval  := corr_floor3.ftval;
		fopen := False;
		t_list[cur_pos].p1 := -randint(10) - 10;
		end;
	end;


//[psect(misc4$code)]
procedure place_secret_door(y, x: Integer);
	var
	cur_pos   : integer;

	begin
	popt(cur_pos);
	with cave[y,x] do
		begin
		tptr := cur_pos;
		t_list[cur_pos] := door_list[3];
		fval  := corr_floor4.ftval;
		fopen := False;
		end;
	end;


//[psect(misc4$code)]
procedure place_door(y, x: Integer);
//	var
//	cur_pos: Integer;

	begin
	case randint(3) of
		1 :
			case randint(4) of
				1:
					place_broken_door(y,x);
				else
					place_open_door(y,x);
				end;
		2 :
			case randint(12) of
				1,2  :
					place_locked_door(y,x);
				3    :
					place_stuck_door(y,x);
				else
					place_closed_door(y,x);
				end;
		3 :
			place_secret_door(y,x);
		end;
	end;



{ Place an up staircase at given y,x    }
//[psect(misc4$code)]
procedure place_up_stairs(y, x: Integer);
	var
	cur_pos: Integer;

	begin
	with cave[y,x] do
		if (tptr <> 0) then
			begin
			pusht(tptr);
			tptr := 0;
			fopen := true;
			end;
	popt(cur_pos);
	cave[y,x].tptr := cur_pos;
	t_list[cur_pos] := up_stair;
	end;

{ Place a down staircase at given y,x   }
//[psect(misc4$code)]
procedure place_down_stairs(y, x: Integer);
	var
	cur_pos    : integer;

	begin
	with cave[y,x] do
		if (tptr <> 0) then
			begin
			pusht(tptr);
			tptr := 0;
			fopen := true;
			end;
	popt(cur_pos);
	cave[y,x].tptr := cur_pos;
	t_list[cur_pos] := down_stair;
	end;

{ Places a staircase 1=up, 2=down   }
//[psect(misc4$code)]
procedure place_stairs(typ, num,walls: Integer);
	var
	i1,
	i2,
	y1,
	x1,
	y2,
	x2: Integer;
	flag: Boolean;

	begin
	for i1 := 1 to num do
		begin
		flag := false;
		repeat
			i2 := 0;
			repeat
				y1 := randint(cur_height - 12);
				x1 := randint(cur_width  - 12);
				y2 := y1 + 12;
				x2 := x1 + 12;
				repeat
					repeat
						with cave[y1,x1] do
							if (fval in [1,2,4]) then
								if (tptr = 0) then
									if (next_to4(y1,x1,wall_set) >= walls) then
										begin
										flag := True;
										case typ of
											1 :
												place_up_stairs(y1,x1);
											2 :
												place_down_stairs(y1,x1);
											end;
										end;
						x1 := x1 + 1;
						until ((x1 = x2) or (flag));
					x1 := x2 - 12;
					y1 := y1 + 1;
					until ((y1 = y2) or (flag));
				i2 := i2 + 1;
				until ((flag) or (i2 > 30));
			walls := walls - 1;
			until(flag);
		end;
	end;

procedure place_gold(AY, AX, ATyp, AQty: Integer); overload;
	var
	cur_pos: Integer;

	begin
	popt(cur_pos);
	cave[AY, AX].tptr:= cur_pos;
	t_list[cur_pos]:= gold_list[ATyp];
	t_list[cur_pos].cost:= AQty;
	end;

{ Places a treasure (Gold or Gems) at given row, column }
//[psect(misc4$code)]
procedure place_gold(y, x: Integer); overload;
	var
//	cur_pos,
	i1,
	i2: Integer;

	begin
//	popt(cur_pos);

	i1 := ((randint(dun_level + 2) + 2) div 2);
	if (randint(obj_great) = 1) then
		i1 := i1 + randint(dun_level);
	if (i1 > max_gold) then
		i1 := max_gold;

//	cave[y,x].tptr := cur_pos;
//	t_list[cur_pos] := gold_list[i1];
//	with t_list[cur_pos] do
//		cost := randint(8*cost) + cost;

	i2:= gold_list[i1].cost;
	Inc(i2, randint(8 * i2));

	place_gold(y, x, i1, i2);
	end;


{ Returns the array number of a random object   }
//[psect(misc4$code)]
function get_obj_num(level: Integer): Integer;
	var
	i1: Integer;

	begin
	if  level > MAX_OBJ_LEVEL  then
		level := MAX_OBJ_LEVEL;

	if  randint(OBJ_GREAT) = 1 then
		level:= MAX_OBJ_LEVEL;

	if  level = 0 then
		i1 := randint(t_level[0])
	else
		i1 := randint(t_level[level]);

	get_obj_num:= i1;
	end;



{ Places an object at given row, column co-ordinate }
//[psect(misc4$code)]
procedure place_object(y, x: Integer);
	var
	cur_pos:    Integer;
//	mag1,
//	mag2: Integer;

	begin
	popt(cur_pos);
	cave[y, x].tptr:= cur_pos;
	t_list[cur_pos]:= object_list[get_obj_num(dun_level)];
	magic_treasure(cur_pos, dun_level);
	end;


{ Allocates an object for tunnels and rooms }
//[psect(misc4$code)]
procedure alloc_object(alloc_set: Obj_Set; typ,num: Integer);
	var
	i1,
	i2,
	i3: Integer;

	begin
	for i3 := 1 to num do
		begin
		repeat
			i1 := randint(cur_height);
			i2 := randint(cur_width);
			until (cave[i1, i2].fval in alloc_set) and (cave[i1, i2].tptr = 0);

		case typ of
			1 :
				place_trap(i1, i2, 1, randint(max_trapa));
			2 :
				place_trap(i1, i2, 2, randint(max_trapb));
			3 :
				place_rubble(i1, i2);
			4 :
				place_gold(i1, i2);
			5 :
				place_object(i1, i2)
			end
		end
	end;


{ Creates objects nearby the coordinates given  }
//[psect(misc4$code)]
procedure random_object(y, x, num: Integer);
	var
	i1,
	i2,
	i3: Integer;

	begin
	repeat
		i1:= 0;
		repeat
			i2:= y - 3 + randint(5);
			i3:= x - 4 + randint(7);
			with cave[i2, i3] do
				if (fval in floor_set) then
					if (tptr = 0) then
						begin
						if (randint(100) < 75) then
							place_object(i2, i3)
						else
							place_gold(i2, i3);
						i1:= 9;
						end;
			i1:= i1 + 1;
			until (i1 > 10);
		num:= num - 1;
		until (num = 0);
	end;


{ Converts stat num into string }
//[psect(misc5$code)]
procedure cnv_stat(stat: ByteInt; var out_val: Stat_Type);
	var
	tmp_str: VType;
	part1,
	part2: Integer;

	begin
	if (stat > 18) then
		begin
		part1:= 18;
		part2:= stat - 18;
		tmp_str := ShortString(IntToStr(part1) + '/' + IntToStr(part2));
		end
	else
		tmp_str:= ShortString(IntToStr(stat));

	if (Length(tmp_str) < 6) then
		tmp_str:= PadRight(tmp_str, 6);

//	out_val    := tmp_str;
	Move(tmp_str[1], out_val[1], 6);
	end;


{ Print character stat in given row, column }
//[psect(misc5$code)]
procedure prt_stat(stat_name: VType; stat: ByteInt; row, column: Integer;
		const AColr: AnsiChar = COLR_NORMAL);
	var
	out_val1: Stat_Type;
	out_val2: VType;
	cs: AnsiString;

	begin
	cnv_stat(stat, out_val1);
	out_val2:= stat_name + out_val1;
	cs:= StringOfChar(COLR_NMHHLT, Length(stat_name)) + AColr;
	put_buffer(out_val2, cs, row, column);
	end;


{ Print character info in given row, column }
//[psect(misc5$code)]
procedure prt_field(info: VType; row, column: Integer);
//	var
//	out_val1,
//	out_val2: VType;

	begin
	put_buffer(PadRight(info, 14), STR_CLR_NORMAL, row,column)
	end;


{ Print number with header at given row, column }
//[psect(misc5$code)]
procedure prt_num(header: VType; num, row, column: Integer;
		const AClr: AnsiChar = COLR_NORMAL);
	var
	out_val: VType;
	cs: AnsiString;

	begin
	out_val := ShortString(string(header) + IntToStr(num) + ' ');
	cs:= StringOfChar(COLR_NMHHLT, Length(header)) + AClr;

	put_buffer(out_val, cs, row, column);
	end;


{ Adjustment for wisdom }
//[psect(misc2$code)]
function wis_adj: Integer;
	begin
	if (py.stat.cwis > 117) then
		wis_adj:= 7
	else if (py.stat.cwis > 107) then
		wis_adj:= 6
	else if (py.stat.cwis > 87) then
		wis_adj:= 5
	else if (py.stat.cwis > 67) then
		wis_adj:= 4
	else if (py.stat.cwis > 17) then
		wis_adj:= 3
	else if (py.stat.cwis > 14) then
		wis_adj:= 2
	else if (py.stat.cwis > 7) then
		wis_adj:= 1
	else
		wis_adj:= 0;
	end;


{ adjustment for intellegence   }
//[psect(misc2$code)]
function int_adj: Integer;
	begin
	if (py.stat.cint > 117) then
		int_adj:= 7
	else if (py.stat.cint > 107) then
		int_adj:= 6
	else if (py.stat.cint > 87) then
		int_adj:= 5
	else if (py.stat.cint > 67) then
		int_adj:= 4
	else if (py.stat.cint > 17) then
		int_adj:= 3
	else if (py.stat.cint > 14) then
		int_adj:= 2
	else if (py.stat.cint > 7) then
		int_adj:= 1
	else
		int_adj:= 0;
	end;


{ Adjustment for charisma
  Percent decrease or increase in price of goods    }
//[psect(misc2$code)]
function chr_adj: Real;
	begin
	if (py.stat.cchr > 117) then
		chr_adj:= -0.10
	else if (py.stat.cchr > 107) then
		chr_adj:= -0.08
	else if (py.stat.cchr > 87) then
		chr_adj:= -0.06
	else if (py.stat.cchr > 67) then
		chr_adj:= -0.04
	else if (py.stat.cchr > 18) then
		chr_adj:= -0.02
	else
		case py.stat.cchr of
			18:
				chr_adj:= 0.00;
			17:
				chr_adj:= 0.01;
			16:
				chr_adj:= 0.02;
			15:
				chr_adj:= 0.03;
			14:
				chr_adj:= 0.04;
			13:
				chr_adj:= 0.06;
			12:
				chr_adj:= 0.08;
			11:
				chr_adj:= 0.10;
			10:
				chr_adj:= 0.12;
			9:
				chr_adj:= 0.14;
			8:
				chr_adj:= 0.16;
			7:
				chr_adj:= 0.18;
			6:
				chr_adj:= 0.20;
			5:
				chr_adj:= 0.22;
			4:
				chr_adj:= 0.24;
			3:
				chr_adj:= 0.25;
			else
				chr_adj:= 0.00; { Error trap }
			end;
	end;


{ Returns a character's adjustment to hit points    }
//[psect(misc2$code)]
function con_adj: Integer;
	begin
	with py.stat do
		if (ccon = 3) then
			con_adj:= -4
		else if (ccon = 4) then
			con_adj:= -3
		else if (ccon = 5) then
			con_adj:= -2
		else if (ccon = 6) then
			con_adj:= -1
		else if (ccon < 17) then
			con_adj:= 0
		else if (ccon = 17) then
			con_adj:= 1
		else if (ccon < 94) then
			con_adj:= 2
		else if (ccon < 117) then
			con_adj:= 3
		else
			con_adj:= 4
	end;


{ Calculates hit points for each level that is gained.  }
//[psect(misc2$code)]
function get_hitdie : Integer;
//	var
//	i1: Integer;

	begin
	get_hitdie := randint(py.misc.hitdie) + con_adj;
	end;


{ Prints title of character }
//[psect(misc5$code)]
procedure prt_title;
	begin
	prt_field(py.misc.title,5,stat_column);
	end;


function GetStatGrade(ACurr, AMax, AAdj: Integer): TMoriaStatGrade;
	begin
	if ACurr < AMax then
		Result:= msgLessThan
	else if AAdj > 0 then
		Result:= msgGreaterThan
	else
		Result:= msgEqualTo;
	end;

procedure CalculateStatAdjustments;
	var
	i1: Integer;

	begin
	with py.stat do
		begin
		astr:= 0;
		adex:= 0;
		acon:= 0;
		aint:= 0;
		awis:= 0;
		achr:= 0;
		end;

	for i1:= Ord(Low(TMoriaInvenWear)) to Ord(High(TMoriaInvenWear)) do
		with newInventory[mitWornWielding, i1] do
			if tval > 0 then
				if (flags and $0001) <> 0 then
					Inc(py.stat.astr, p1)
				else if (flags and $0002) <> 0 then
					Inc(py.stat.adex, p1)
				else if (flags and $0004) <> 0 then
					Inc(py.stat.acon, p1)
				else if (flags and $0008) <> 0 then
					Inc(py.stat.aint, p1)
				else if (flags and $0010) <> 0 then
					Inc(py.stat.awis, p1)
				else if (flags and $0020) <> 0 then
					Inc(py.stat.achr, p1);
	end;

{ Prints strength   }
//[psect(misc5$code)]
procedure prt_strength;
	begin
	with py.stat do
		prt_stat('', cstr, 7, stat_column + 6,
				ARR_CLR_STATGRD[GetStatGrade(cstr, str, astr)]);
	end;


{ Prints intelligence   }
//[psect(misc5$code)]
procedure prt_intelligence;
	begin
	with py.stat do
		prt_stat('', cint, 8, stat_column + 6,
				ARR_CLR_STATGRD[GetStatGrade(cint, int, aint)]);
	end;


{ Prints wisdom }
//[psect(misc5$code)]
procedure prt_wisdom;
	begin
	with py.stat do
		prt_stat('', cwis, 9, stat_column + 6,
				ARR_CLR_STATGRD[GetStatGrade(cwis, wis, awis)]);
	end;


{ Prints dexterity  }
//[psect(misc5$code)]
procedure prt_dexterity;
	begin
	with py.stat do
		prt_stat('', cdex, 10, stat_column + 6,
				ARR_CLR_STATGRD[GetStatGrade(cdex, dex, adex)]);
	end;


{ Prints constitution   }
//[psect(misc5$code)]
procedure prt_constitution;
	begin
	with py.stat do
		prt_stat('', ccon, 11, stat_column + 6,
				ARR_CLR_STATGRD[GetStatGrade(ccon, con, acon)]);
	end;


{ Prints charisma   }
//[psect(misc5$code)]
procedure prt_charisma;
	begin
	with py.stat do
		prt_stat('', cchr, 12, stat_column + 6,
				ARR_CLR_STATGRD[GetStatGrade(cchr, chr, achr)]);
	end;



{ Prints level  }
//[psect(misc5$code)]
procedure prt_level;
	begin
	prt_num('',py.misc.lev,14,stat_column+6);
	end;


{ Prints players current (a real number...) and max mana points }
//[psect(misc1$code)]
procedure prt_mana;
	var
	mwkdiv: Real;
	pc: Real;
	clr: AnsiChar;

	begin
//dengland We're sometimes getting here even if there isn't any mana for the class.
	if py.misc.mana > 0 then
		pc:= py.misc.cmana / py.misc.mana
	else
		pc:= 1;

	if pc >= 0.8 then
		clr:= COLR_GOODPC
	else if pc >= 0.3 then
		clr:= COLR_FAIRPC
	else
		clr:= COLR_POORPC;

	mwkdiv:= py.misc.cmana / 10;
	if (mwkdiv < 1) then
		indx:=2
	else if (mwkdiv < 10) then
		indx:=3
	else
		indx:=4;
	prt_num('',Trunc(py.misc.cmana),18,stat_column+6, clr);
	prt_num('/',py.misc.mana,18,stat_column+5+indx);
	end;


{ Prints Max hit points }
//[psect(misc5$code)]
procedure prt_mhp;
	begin
	prt_num('',py.misc.mhp,17,stat_column+6);
	end;


{ Prints players current hit points (a real number...)  }
//[psect(misc1$code)]
procedure prt_chp;
	var
	pc: Real;
	clr: AnsiChar;

	begin
	if py.misc.mhp > 0 then
		pc:= py.misc.chp / py.misc.mhp
	else
		pc:= 1;

	if pc >= 0.8 then
		clr:= COLR_GOODPC
	else if pc >= 0.3 then
		clr:= COLR_FAIRPC
	else
		clr:= COLR_POORPC;

	prt_num('',Trunc(py.misc.chp),16,stat_column+6, clr);
	end;


{ prints current AC }
//[psect(misc5$code)]
procedure prt_pac;
	begin
	prt_num('',py.misc.dis_ac,19,stat_column+6);
	end;


{ Prints current gold   }
//[psect(misc5$code)]
procedure prt_gold;
	begin
	prt_num( '',py.misc.au,20,stat_column+6);
	end;

{ Prints current weight   }
//[psect(misc5$code)]
procedure prt_cur_weight;
	var
	pc: Real;
	clr: AnsiChar;

	begin
	max_weight:= weight_limit;
	pc:= inven_weight / max_weight;
	if pc >= 0.8 then
		clr:= COLR_POORPC
	else if pc >= 0.3 then
		clr:= COLR_FAIRPC
	else
		clr:= COLR_GOODPC;

	prt_num( '',inven_weight,21,stat_column+6, clr);
	end;

{ Prints weight capacity }
//[psect(misc5$code)]
procedure prt_max_weight;
	begin
	max_weight:=weight_limit;
	prt_num( '',max_weight,22,stat_column+6);
	end;

{ Prints depth in stat area }
//[psect(misc5$code)]
procedure prt_depth;
	var
	depths: VType;
	depth:  Integer;

	begin
	depth:= dun_level * 50;
	if (depth = 0) then
		depths:= 'Town level'
	else
		depths:= ShortString('Depth: ' + IntToStr(depth) + ' (feet)');

	prt(depths, 24, 61);
	end;


{ Prints status of hunger   }
//[psect(misc5$code)]
procedure prt_hunger;
	begin
	if (($000002 and py.flags.status) <> 0) then
		begin
		put_buffer('Weak    ', AnsiChar(attrFGRed), 24,1);
		end
	else if (($000001 and py.flags.status) <> 0) then
		begin
		put_buffer('Hungry  ', AnsiChar(attrFGRed), 24,1);
		end
	else
		put_buffer('        ', STR_CLR_NORMAL, 24,1);
	end;


{ Prints Blind status   }
//[psect(misc5$code)]
procedure prt_blind;
	begin
	if (($000004 and py.flags.status) <> 0) then
		begin
		put_buffer('Blind  ', AnsiChar(attrFGBlue), 24,9);
		end
	else
		put_buffer('       ', STR_CLR_NORMAL, 24,9);
	end;


{ Prints Confusion status   }
//[psect(misc5$code)]
procedure prt_confused;
	begin
	if (($000008 and py.flags.status) <> 0) then
		begin
		put_buffer('Confused  ',AnsiChar(attrFGBrown), 24,16);
		end
	else
		put_buffer('          ', STR_CLR_NORMAL, 24,16);
	end;


{ Prints Fear status    }
//[psect(misc5$code)]
procedure prt_afraid;
	begin
	if (($000010 and py.flags.status) <> 0) then
		begin
		put_buffer('Afraid  ', AnsiChar(attrFGBrown), 24,26);
		end
	else
		put_buffer('        ', STR_CLR_NORMAL, 24,26);
	end;


{ Prints Poisoned status }
//[psect(misc5$code)]
procedure prt_poisoned;
	begin
	if (($000020 and py.flags.status) <> 0) then
		begin
		put_buffer('Poisoned  ', AnsiChar(attrFGGreen), 24,34);
		end
	else
		put_buffer('          ', STR_CLR_NORMAL, 24,34);
	end;


{ Prints Searching status   }
//[psect(misc5$code)]
procedure prt_search;
	begin
	if (($000100 and py.flags.status) <> 0) then
		begin
		put_buffer('Searching  ',AnsiChar(attrFGCyan), 24,44);
		end
	else
		put_buffer('           ', STR_CLR_NORMAL, 24,44);
	end;


{ Prints Resting status }
//[psect(misc5$code)]
procedure prt_rest;
	begin
	if (($000200 and py.flags.status) <> 0) then
		begin
		put_buffer('Resting    ',AnsiChar(attrFGLightGray), 24,44);
		end
	else
		put_buffer('           ', STR_CLR_NORMAL, 24,44);
	end;


{ Prints winner status on display   }
//[psect(misc5$code)]
procedure prt_winner;
	begin
	put_buffer('*Winner*',AnsiChar(attrFGYellow), 23,1);
	end;


{ Increases a stat by one randomized level  }
//[psect(misc2$code)]
function in_statp(stat: ByteInt): ByteInt;
	begin
	if (stat < 18) then
		stat := stat + 1
	else if (stat < 88) then
		stat := stat + randint(25)
	else if (stat < 108) then
		stat := stat + randint(10)
	else
		stat := stat + 1;

	if (stat > 118) then
		stat := 118;

	in_statp := stat;
	end;


{ Decreases a stat by one randomized level  }
//[psect(misc2$code)]
function de_statp(stat: ByteInt): ByteInt;
	begin
	if (stat < 19) then
		stat := stat - 1
	else if (stat < 109) then
		begin
		stat := stat - randint(10) - 5;
		if (stat < 18) then
			stat := 18;
		end
	else
		stat := stat - randint(3);

	if (stat < 3) then
		stat := 3;

	de_statp := stat;
	end;


{ Increases a stat by one true level    }
//[psect(misc2$code)]
function in_statt(stat: ByteInt): ByteInt;
	begin
	if (stat < 18) then
		stat := stat + 1
	else
		begin
		stat := stat + 10;
		if (stat > 118) then
			stat := 118;
		end;

	in_statt := stat;
	end;


{ Decreases a stat by true level    }
//[psect(misc2$code)]
function de_statt(stat: ByteInt): ByteInt;
	begin
	if (stat > 27) then
		stat := stat - 10
	else if (stat > 18) then
		stat := 18
	else
		begin
		stat := stat - 1;
		if (stat < 3) then
			stat := 3;
		end;

	de_statt := stat;
	end;


{ Returns a character's adjustment to hit.  }
//[psect(misc2$code)]
function tohit_adj: Integer;
	var
	total: Integer;

	begin
	with py.stat do
		begin
		if (cdex <   4) then
			total := -3
		else if (cdex <   6) then
			total := -2
		else if (cdex <   8) then
			total := -1
		else if (cdex <  16) then
			total :=  0
		else if (cdex <  17) then
			total :=  1
		else if (cdex <  18) then
			total :=  2
		else if (cdex <  69) then
			total :=  3
		else if (cdex < 118) then
			total :=  4
		else
			total :=  5;

		if (cstr <   4) then
			total := total - 3
		else if (cstr <   5) then
			total := total - 2
		else if (cstr <   7) then
			total := total - 1
		else if (cstr <  18) then
			total := total + 0
		else if (cstr <  94) then
			total := total + 1
		else if (cstr < 109) then
			total := total + 2
		else if (cstr < 117) then
			total := total + 3
		else
			total := total + 4;
		end;

	tohit_adj := total;
	end;


{ Returns a character's adjustment to armor class   }
//[psect(misc2$code)]
function toac_adj: Integer;
	begin
	with py.stat do
		if (cdex < 4) then
			toac_adj:= -4
		else if (cdex = 4) then
			toac_adj:= -3
		else if (cdex = 5) then
			toac_adj:= -2
		else if (cdex = 6) then
			toac_adj:= -1
		else if (cdex < 15) then
			toac_adj:= 0
		else if (cdex < 18) then
			toac_adj:= 1
		else if (cdex < 59) then
			toac_adj:= 2
		else if (cdex < 94) then
			toac_adj:= 3
		else if (cdex < 117) then
			toac_adj:= 4
		else
			toac_adj:= 5
	end;


{ Returns a character's adjustment to disarm    }
//[psect(misc2$code)]
function todis_adj: Integer;
	begin
	with py.stat do
		if (cdex = 3) then
			todis_adj:= -8
		else if (cdex = 4) then
			todis_adj:= -6
		else if (cdex = 5) then
			todis_adj:= -4
		else if (cdex = 6) then
			todis_adj:= -2
		else if (cdex = 7) then
			todis_adj:= -1
		else if (cdex < 13) then
			todis_adj:= 0
		else if (cdex < 16) then
			todis_adj:= 1
		else if (cdex < 18) then
			todis_adj:= 2
		else if (cdex < 59) then
			todis_adj:= 4
		else if (cdex < 94) then
			todis_adj:= 5
		else if (cdex < 117) then
			todis_adj:= 6
		else
			todis_adj:= 8
	end;


{ Returns a character's adjustment to damage    }
//[psect(misc2$code)]
function todam_adj: Integer;
	begin
	with py.stat do
		if (cstr < 4) then
			todam_adj:= -2
		else if (cstr < 5) then
			todam_adj:= -1
		else if (cstr < 16) then
			todam_adj:= 0
		else if (cstr < 17) then
			todam_adj:= 1
		else if (cstr < 18) then
			todam_adj:= 2
		else if (cstr < 94) then
			todam_adj:= 3
		else if (cstr < 109) then
			todam_adj:= 4
		else if (cstr < 117) then
			todam_adj:= 5
		else
			todam_adj:= 6;
	end;


{ Pre-declaration of PRT_EXPERIENCE }
//[psect(misc5$code)]
procedure prt_experience; forward;


{ Prints character-screen info  }
//[psect(misc5$code)]
procedure prt_stat_block;
	var
	indx:   Integer;
	mwkdiv: Real;
	clr: AnsiChar;
	pc: Real;

	begin
	prt_field(py.misc.race, 3, stat_column);
	prt_field(py.misc.tclass, 4, stat_column);
	prt_field(py.misc.title, 5, stat_column);

	with py.stat do
		begin
		prt_stat('STR : ', cstr, 7, stat_column,
				ARR_CLR_STATGRD[GetStatGrade(cstr, str, astr)]);
		prt_stat('INT : ', cint, 8, stat_column,
				ARR_CLR_STATGRD[GetStatGrade(cint, int, aint)]);
		prt_stat('WIS : ', cwis, 9, stat_column,
				ARR_CLR_STATGRD[GetStatGrade(cwis, wis, awis)]);
		prt_stat('DEX : ', cdex, 10, stat_column,
				ARR_CLR_STATGRD[GetStatGrade(cdex, dex, adex)]);
		prt_stat('CON : ', ccon, 11, stat_column,
				ARR_CLR_STATGRD[GetStatGrade(ccon, con, acon)]);
		prt_stat('CHR : ', cchr, 12, stat_column,
				ARR_CLR_STATGRD[GetStatGrade(cchr, chr, achr)]);
		end;

	prt_num('LEV : ', py.misc.lev, 14, stat_column);
	prt_num('EXP : ', py.misc.exp, 15, stat_column);

	pc:= py.misc.chp / py.misc.mhp;
	if pc >= 0.8 then
		clr:= COLR_GOODPC
	else if pc >= 0.3 then
		clr:= COLR_FAIRPC
	else
		clr:= COLR_POORPC;

	prt_num('CHP : ', Trunc(py.misc.chp), 16, stat_column, clr);
	prt_num('MHP : ', py.misc.mhp, 17, stat_column);

	if  ((py.misc.tclass <> 'Rogue')
	and  (py.misc.tclass <> 'Warrior')) then
		begin
		if py.misc.mana > 0 then
			pc:= py.misc.cmana / py.misc.mana
		else
			pc:= 1;

		if pc >= 0.8 then
			clr:= COLR_GOODPC
		else if pc >= 0.3 then
			clr:= COLR_FAIRPC
		else
			clr:= COLR_POORPC;

		mwkdiv:= py.misc.cmana / 10;
		if (mwkdiv < 1) then
			indx:= 2
		else if (mwkdiv < 10) then
			indx:= 3
		else
			indx:= 4;
		prt_num('MANA: ', Trunc(py.misc.cmana), 18, stat_column, clr);
		prt_num('/', py.misc.mana, 18, stat_column + 5 + indx);
		end;

	prt_num('AC  : ', py.misc.dis_ac, 19, stat_column);
	prt_num('GOLD: ', py.misc.au, 20, stat_column);

	max_weight:= weight_limit;

	if max_weight = 0 then
		pc:= 1
	else
		pc:= inven_weight / max_weight;

	if pc >= 0.8 then
		clr:= COLR_POORPC
	else if pc >= 0.3 then
		clr:= COLR_FAIRPC
	else
		clr:= COLR_GOODPC;

	prt_num('WGHT: ', inven_weight, 21, stat_column, clr);
	prt_num('MXWT: ', max_weight, 22, stat_column);

	if (total_winner) then
		prt_winner;

	if (($000003 and py.flags.status) <> 0) then
		prt_hunger;
	if (($000004 and py.flags.status) <> 0) then
		prt_blind;
	if (($000008 and py.flags.status) <> 0) then
		prt_confused;
	if (($000010 and py.flags.status) <> 0) then
		prt_afraid;
	if (($000020 and py.flags.status) <> 0) then
		prt_poisoned;
	if (($000100 and py.flags.status) <> 0) then
		prt_search;
	if (($000200 and py.flags.status) <> 0) then
		prt_rest;
	end;


{ Draws entire screen   }
//[psect(misc5$code)]
procedure draw_cave;
	begin
	screen.BeginUpdate;
	try
		clear(1,1);
		prt_stat_block;
		prt_map;
		prt_depth;

		finally
		screen.EndUpdate;
		end;
	end;


{ Prints the following information on the screen.   }
//[psect(create$code)]
procedure put_character;
	var
	cs: AnsiString;

	begin
	clear(1,1);

	cs:= StringOfChar(COLR_NMHHLT, 7) + COLR_NORMAL;

	with py.misc do
		begin
		prt('Name  : ' + name, cs, 3,3);
		prt('Race  : ' + race, cs, 4,3);
		prt('Sex   : ' + sex, cs, 5,3);
		prt('Class : ' + tclass, cs, 6,3)
		end
	end;


{ Prints the following information on the screen. }
//[psect(create$code)]
procedure put_stats;
	begin
	with py do
		begin
		prt('Cur', STR_CLR_NMHHLT, 2,62);
		prt('Max', STR_CLR_NMHHLT, 2,71);
		prt_stat('STR : ',stat.cstr,3,56);
		prt_stat('| ', stat.str,3,69);
		prt_stat('INT : ',stat.cint,4,56);
		prt_stat('| ',stat.int,4,69);
		prt_stat('WIS : ',stat.cwis,5,56);
		prt_stat('| ',stat.wis,5,69);
		prt_stat('DEX : ',stat.cdex,6,56);
		prt_stat('| ',stat.dex,6,69);
		prt_stat('CON : ',stat.ccon,7,56);
		prt_stat('| ',stat.con,7,69);
		prt_stat('CHR : ',stat.cchr,8,56);
		prt_stat('| ',stat.chr,8,69);
		prt_num('+ To hit : ',misc.dis_th,10,4);
		prt_num('+ To dam : ',misc.dis_td,11,4);
		prt_num('+ To AC  : ',misc.dis_tac,12,4);
		prt_num('Total AC : ',misc.dis_ac,13,4);
		end
	end;


{ Returns a rating of x depending on y }
//[psect(create$code)]
function likert(x, y: Integer): BType;
	begin
	case x div y of
		-3,-2,-1      :
			likert := 'Very bad';
		0,1    :
			likert := 'Bad';
		2      :
			likert := 'Poor';
		3,4    :
			likert := 'Fair';
		5      :
			likert := 'Good';
		6      :
			likert := 'Very good';
		7,8    :
			likert := 'Superb';
		else
			likert := 'Excellent';
		end
	end;


{ Prints age, height, weight, and SC }
//[psect(create$code)]
procedure put_misc1;
	begin
	with py do
		begin
		prt_num('Age          : ',misc.age,3,36);
		prt_num('Height       : ',misc.ht ,4,36);
		prt_num('Weight       : ',misc.wt ,5,36);
		prt_num('Social class : ',misc.sc ,6,36);
		end;
	end;


{ Prints the following information on the screen. }
//[psect(create$code)]
procedure put_misc2;
	begin
	with py.misc do
		begin
		prt_num('Level          : ',lev ,10,27);
		prt_num('Cur experience : ',exp ,11,27);
		prt_num('Max experience : ',max_exp,12,27);
		prt_num('Gold           : ',au  ,13,27);
		prt_num('Max hit points : ',mhp ,10,54);
		prt_num('Cur hit points : ',trunc(chp),11,54);
		prt_num('Max mana       : ',mana ,12,54);
		prt_num('Current mana   : ',trunc(cmana),13,54);
		end
	end;


{ Prints ratings on certain abilities }
//[psect(create$code)]
procedure put_misc3;
	var
	xbth,
	xbthb,
	xfos,
	xsrh,
	xstl,
	xdis,
	xsave,
	xdev: Integer;
	xinfra: VType;
	cs: AnsiString;

	begin
	clear(14,1);
	with py.misc do
		begin
		xbth  := bth + lev*bth_lev_adj + ptohit*bth_plus_adj;
		xbthb := bthb + lev*bth_lev_adj + ptohit*bth_plus_adj;
		xfos  := 27 - fos;
		if (xfos < 0) then
			xfos := 0;
		xsrh  := srh + int_adj;
		xstl  := stl;
		xdis  := disarm + lev + 2*todis_adj + int_adj;
		xsave := save + lev + wis_adj;
		xdev  := save + lev + int_adj;
		xinfra:= ShortString(IntToStr(py.flags.see_infra * 10) + ' feet');
		end;

	cs:= StringOfChar(COLR_NMHHLT, 13) + COLR_NORMAL;

	prt('(Miscellaneous Abilities)', STR_CLR_NMHHLT, 16,24);
	put_buffer('Fighting    : '+likert(xbth ,12), cs, 17, 2);
	put_buffer('Bows/Throw  : '+likert(xbthb,12), cs, 18, 2);
	put_buffer('Saving throw: '+likert(xsave, 6), cs, 19, 2);
	put_buffer('Stealth     : '+likert(xstl , 1), cs, 17,27);
	put_buffer('Disarming   : '+likert(xdis , 8), cs, 18,27);
	put_buffer('Magic device: '+likert(xdev , 7), cs, 19,27);
	put_buffer('Perception  : '+likert(xfos , 3), cs, 17,52);
	put_buffer('Searching   : '+likert(xsrh , 6), cs, 18,52);
	put_buffer('Infra-vision: '+xinfra,           cs, 19,52);
	end;


{ Used to display the character on the screen. }
//[psect(create$code)]
procedure display_char;
//	var
//	dummy: AnsiChar;

	begin
	put_character;
	put_misc1;
	put_stats;
	put_misc2;
	put_misc3;
	end;


{ Destroy an item in the inventory }
//[psect(misc4$code)]
//todo Remove
(*procedure inven_destroy(item_val: Integer);
	var
	i2: Integer;

	begin
	inventory[INVEN_MAX] := inventory[item_val];
	with inventory[item_val] do
		begin
		if ((number > 1) and (subval < 512))  then
			begin
			number := number - 1;
			inven_weight := inven_weight - weight;
			inventory[inven_max].number := 1;
			end
		else
			begin
			inven_weight := inven_weight - weight*number;
			for i2 := item_val to inven_ctr-1 do
				inventory[i2] := inventory[i2+1];
			inventory[inven_ctr] := blank_treasure;
			inven_ctr := inven_ctr - 1;
			end;
		end
	end;*)


{ Drops an item from inventory to given location }
//[psect(misc4$code)]
//todo Remove
(*procedure inven_drop(item_val, y, x: Integer);
	var
	i1: Integer;

	begin
	with cave[y,x] do
		begin
		if (tptr > 0) then
			pusht(tptr);
		inven_destroy(item_val);
		popt(i1);
		t_list[i1] := inventory[INVEN_MAX];
		tptr := i1;
		end;
	end;*)

{ Destroys a type of item on a given percent chance }
//[psect(misc4$code)]
//todo Remove
(*function inven_damage(typ: Obj_Set; perc: Integer): Integer;
	var
	i1,
	i2: Integer;

	begin
	i2 := 0;
	for i1 := 1 to inven_ctr do
		with inventory[i1] do
			if (tval in typ) then
				if (randint(100) < perc) then
					begin
					inven_destroy(i1);
					i2 := i2 + 1;
					end;

	inven_damage := i2;
	end;*)

{ Check inventory for too much weight   }
//[psect(misc4$code)]
//todo Remove
(*function inven_check_weight(t: PTreasure_Type): Boolean;
	var
	item_wgt : integer;

	begin
	Result:= False;
	max_weight := weight_limit;
	with t^ do
		item_wgt := number * weight;
{ Now, check to see if player can carry object  }
	if ((inven_weight + item_wgt) <= max_weight) then
		Result:= True;   { Can carry weight  }
	end;*)


{ Check to see if he will be carrying too many objects  }
//[psect(misc4$code)]
//todo Remove
(*function inven_check_num: Boolean;
	var
//	item_num,
	i1: Integer;
//	flag: Boolean;

	begin
	inven_check_num := False;
	if (inven_ctr < 22) then
		inven_check_num := True
	else if (inventory[inven_max].subval > 255) then
		for i1 := 1 to inven_ctr do
			with inventory[i1] do
				if (tval = inventory[inven_max].tval) then
					if (subval = inventory[inven_max].subval) then
						inven_check_num := True;
	end;*)


{ Add the item in INVEN_MAX to players inventory.  Return the
  item position for a description if needed...  }
//[psect(misc4$code)]
//todo Remove
(*procedure inven_carry(var item_val: Integer);
	var
	item_num,
	wgt,
	typ,
	subt: Integer;
	flag: Boolean;
	i,
	itype: TMoriaInvenType;
	p: Integer;

{ Insert INVEN_MAX at given location }
	procedure insert(pos,wgt : integer);
		var
		i1: Integer;

		begin
		for i1 :=  inven_ctr downto pos do
			inventory[i1+1] := inventory[i1];
		inventory[pos] := inventory[inven_max];
		inven_ctr      := inven_ctr   + 1;
		inven_weight   := inven_weight + wgt;
		end;

	procedure insertNew(AType: TMoriaInvenType; APos, AWgt: Integer);
		var
		i,
		l: Integer;

		begin
		l:= Length(newInventory[AType]);
		SetLength(newInventory[AType], l + 1);

		if APos < l then
			for i:= l downto APos + 1 do
				newInventory[AType, i]:= newInventory[AType, i - 1];

		newInventory[AType, APos]:= inventory[INVEN_MAX];
//		Inc(inven_weight, AWgt);
		end;

{ INVEN_CARRY routine }
	begin
{ Now, check to see if player can carry object  }
	item_val := 0;
	flag := False;
	with inventory[INVEN_MAX] do
		begin
		item_num := number;
		typ  := tval;
		subt := subval;
		wgt  := number*weight;
		end;

	repeat
		item_val := item_val + 1;
		with inventory[item_val] do
			if (typ = tval) then
				begin
				if (subt = subval) then { Adds to other item }
					if (subt > 255) then
						begin
						number := number + item_num;
						inven_weight := inven_weight + wgt;
						flag := True;
						end;
				end
			else if (typ > tval) then
				begin     { Insert into list  }
				insert(item_val,wgt);
				flag := True;
				end;
		until ((item_val >= inven_ctr) or (flag));

	if (not(flag)) then     { Becomes last item in list }
		begin
		insert(inven_ctr+1,wgt);
		item_val := inven_ctr;
		end;

//	Find the group to put it in

//	Default
	itype:= mitGeneral;

//	Check for books first
	if  typ in ARR_FLG_INVSUBVAL[mitBooks] then
		itype:= mitBooks
	else
//		Check from mitWeapons..mitConsumables
		for i:= mitWeapons to mitConsumables do
			if  typ in ARR_FLG_INVSUBVAL[i] then
				begin
				itype:= i;
				Break;
				end;

//	Find the place to put it or stack
	flag:= False;
	for p:= 0 to High(newInventory[itype]) do
		begin
		with newInventory[itype, p] do
			if (typ = tval) then
				begin
				if (subt = subval) then { Adds to other item }
					if (subt > 255) then
						begin
						number:= number + item_num;
//						Inc(inven_weight, wgt);
						flag:= True;
						Break;
						end;
				end
			else if (typ > tval) then
				begin     { Insert into list  }
				insertNew(itype, p, wgt);
				flag:= True;
				Break;
				end;
		end;

	if  not flag then     { Becomes last item in list }
		begin
		insertNew(itype, Length(newInventory[itype]), wgt);
//		item_val := inven_ctr;
		end;
	end;*)


{ Returns spell chance of failure for spell }
//[psect(misc2$code)]
procedure spell_chance(var spell: Spl_Rec);
	begin
	if (class_[py.misc.pclass].mspell) then
		with mage_spell[py.misc.pclass,spell.splnum] do
			with spell do
				begin
				splchn := sfail - 3*(py.misc.lev-slevel);
				if (class_[py.misc.pclass].mspell) then
					splchn := splchn - 3*(int_adj-1)
				else
					splchn := splchn - 3*(wis_adj-1);

				if (smana > py.misc.cmana) then
					splchn := splchn + 5 * Trunc(smana-py.misc.cmana);

				if (splchn > 95) then
					splchn := 95
				else if (splchn < 0) then
					splchn := 0;
				end ;

	if (class_[py.misc.pclass].pspell) then
		with priest_spell[py.misc.pclass,spell.splnum] do
			with spell do
				begin
				splchn := sfail - 3*(py.misc.lev-slevel);
				if (class_[py.misc.pclass].mspell) then
					splchn := splchn - 3*(int_adj-1)
				else
					splchn := splchn - 3*(wis_adj-1);

				if (smana > py.misc.cmana) then
					splchn := splchn + 5 * Trunc(smana-py.misc.cmana);

				if (splchn > 95) then
					splchn := 95
				else if (splchn < 0) then
					splchn := 0;
				end;

	if (class_[py.misc.pclass].espell) then
		with extra_spell[py.misc.pclass,spell.splnum] do
			with spell do
				begin
				splchn := sfail - 3*(py.misc.lev-slevel);
				if (class_[py.misc.pclass].mspell) then
					splchn := splchn - 3*(int_adj-1)
				else
					splchn := splchn - 3*(wis_adj-1);

				if (smana > py.misc.cmana) then
					splchn := splchn + 5 * Trunc(smana-py.misc.cmana);

				if (splchn > 95) then
					splchn := 95
				else if (splchn < 0) then
					splchn := 0;
				end
	end;


{ Print list of spells  }
//[psect(misc2$code)]
procedure print_new_spells(spell: Spl_Type; num: Integer; var redraw: Boolean);
	var
	i1: Integer;
	out_val: VType;

	begin
	redraw := True;
	clear(1,1);
	prt('   Name                          Level  Mana  %Failure',2,1);
	for i1 := 1 to num do
		if (class_[py.misc.pclass].mspell) then
			with mage_spell[py.misc.pclass,spell[i1].splnum] do
				begin
				spell_chance(spell[i1]);

				out_val:= ShortString(AnsiChar(96 + i1) + ') ' +
						string(PadRight(sname, 30)) +
						Format('%3d    %3d      %2d', [slevel, smana, spell[i1].splchn]));

				prt(out_val,2+i1,1);
				end
		else if (class_[py.misc.pclass].pspell) then
			with priest_spell[py.misc.pclass,spell[i1].splnum] do
				begin
				spell_chance(spell[i1]);

				out_val:= ShortString(AnsiChar(96+i1)+') '+
						string(PadRight(sname, 30)) +
						Format('%3d    %3d      %2d', [slevel, smana, spell[i1].splchn]));
				prt(out_val,2+i1,1);
				end
		else if (class_[py.misc.pclass].espell) then
			with extra_spell[py.misc.pclass,spell[i1].splnum] do
				begin
				spell_chance(spell[i1]);

				out_val:= ShortString(AnsiChar(96+i1)+') '+
						string(PadRight(sname, 30)) +
						Format('%3d    %3d      %2d',
						[slevel, smana, spell[i1].splchn]));
				prt(out_val,2+i1,1);
				end;
	end;

{ Returns spell pointer }
//[psect(misc2$code)]
function get_spell(spell: Spl_Type; num: Integer; var sn, sc: Integer;
		prompt: VType; var redraw: Boolean): Boolean;
	var
//	i1: Integer;
	flag: Boolean;
	choice: AnsiChar;
	out_val1: VType;
//	out_val2: VType;

	begin
	sn := 0;
	flag := True;
	out_val1:= ShortString('(Spells a-' + AnsiChar(num+96) + ', *=List, <ESCAPE=Exit) ' +
			string(prompt));

	while (((sn < 1) or (sn > num)) and (flag)) do
		begin
		prt(out_val1, STR_CLR_NMHHLT, 1, 1);
		inkey(choice);
		sn := ord(choice);
		case sn of
			0,3,25,26,27:
				begin
				flag := False;
//dengland This is the only place that the global variable was used
//fixme Check that this actually shouldn't be done
//				reset_flag := True;
				end;
			42 :
				print_new_spells(spell,num,redraw);
			else
				sn := sn - 96;
			end;
		end;
	msg_flag := False;
	if (flag) then
		begin
		spell_chance(spell[sn]);
		sc := spell[sn].splchn;
		sn := spell[sn].splnum;
		end;
	get_spell := flag;
	end;


{ Learn some magic spells (Mage)    }
//[psect(misc2$code)]
function learn_spell(var redraw: Boolean): Boolean;
	var
	i2: Unsigned;
	i1,
	i3,
	sn,
	sc: Integer;
	new_spells: Integer;
	spell_flag: Unsigned;
	spell: Spl_Type;

	begin
	learn_spell := false;
	case int_adj of
		0 :
			new_spells := 0;
		1 :
			new_spells := 1;
		2 :
			new_spells := 1;
		3 :
			new_spells := 1;
		4 :
			new_spells := randint(2);
		5 :
			new_spells := randint(2);
		6 :
			new_spells := randint(3);
		7 :
			new_spells := randint(2)+1;
		else
			new_spells := 0;
		end;
//	i1 := 0;
	spell_flag := 0;
//	repeat
//		i1 := i1 + 1;
//		if (inventory[i1].tval = 90) then
//			spell_flag := (spell_flag or inventory[i1].flags);
//		until (i1 >= inven_ctr);
	for i1:= 0 to Length(newInventory[mitBooks]) -  1 do
		if  newInventory[mitBooks, i1].tval = 90 then
			spell_flag:= spell_flag or newInventory[mitBooks, i1].flags;

	while ((new_spells > 0) and (spell_flag > 0)) do
		begin
		i1 := 0;
		i2 := spell_flag;
		repeat
			i3 := bit_pos(i2);
			with mage_spell[py.misc.pclass,i3] do
				if (slevel <= py.misc.lev) then
					if (not(learned)) then
						begin
						i1 := i1 + 1;
						spell[i1].splnum := i3;
						end;

			until(i2 = 0);

		if (i1 > 0) then
			begin
			print_new_spells(spell,i1,redraw);
			if (get_spell(spell,i1,sn,sc,'Learn which spell?',redraw)) then
				begin
				mage_spell[py.misc.pclass,sn].learned := true;
				learn_spell := true;
				if (py.misc.mana = 0) then
					begin
					py.misc.mana   := 1;
					py.misc.cmana := 1;
					end;
				end
			else
				new_spells := 0;
			end
		else
			new_spells := 0;
		new_spells := new_spells - 1;
		end;
	end;

{ Learn some extra spells (Consult)    }
//[psect(misc2$code)]
function learn_extra(var redraw : Boolean): Boolean;
	var
	i2  : unsigned;
	i1,
	i3,
	sn,
	sc: Integer;
	new_spells: Integer;
	spell_flag: Unsigned;
	spell: Spl_Type;

	begin
	learn_extra := false;
	case int_adj of
		0 :
			new_spells := 0;
		1 :
			new_spells := 1;
		2 :
			new_spells := 1;
		3 :
			new_spells := 1;
		4 :
			new_spells := randint(2);
		5 :
			new_spells := randint(2);
		6 :
			new_spells := randint(3);
		7 :
			new_spells := randint(2)+1;
		else
			new_spells := 0;
		end;

//	i1 := 0;
	spell_flag := 0;
//	repeat
//		i1 := i1 + 1;
//		if (inventory[i1].tval = 92) then
//			spell_flag := (spell_flag or inventory[i1].flags);
//		until (i1 >= inven_ctr);
	for i1:= 0 to Length(newInventory[mitBooks]) -  1 do
		if  newInventory[mitBooks, i1].tval = 92 then
			spell_flag:= spell_flag or newInventory[mitBooks, i1].flags;

	while ((new_spells > 0) and (spell_flag > 0)) do
		begin
		i1 := 0;
		i2 := spell_flag;
		repeat
			i3 := bit_pos(i2);
			with extra_spell[py.misc.pclass,i3] do
				if (slevel <= py.misc.lev) then
					if (not(learned)) then
						begin
						i1 := i1 + 1;
						spell[i1].splnum := i3;
						end;
			until(i2 = 0);

		if (i1 > 0) then
			begin
			print_new_spells(spell,i1,redraw);
			if (get_spell(spell,i1,sn,sc,'learn which spell?',redraw)) then
				begin
				extra_spell[py.misc.pclass,sn].learned := true;
				learn_extra := true;
				if (py.misc.mana = 0) then
					begin
					py.misc.mana   := 1;
					py.misc.cmana := 1;
					end;
				end
			else
				new_spells := 0;
			end
		else
			new_spells := 0;

		new_spells := new_spells - 1;
		end;
	end;


{ Learn some prayers (Priest)   }
//[psect(misc2$code)]
function learn_prayer: Boolean;
	var
	i1,
	i2,
	i3,
	i4,
	new_spell: Integer;
	test_array: array [1..32] of Integer;
	spell_flag: Unsigned;

	begin
//	i1 := 0;
	i2:= 0;
	spell_flag := 0;
//	repeat
//		i1 := i1 + 1;
//		if (inventory[i1].tval = 91) then
//			spell_flag := (spell_flag or inventory[i1].flags);
//		until(i1 >= inven_ctr);
	for i1:= 0 to Length(newInventory[mitBooks]) -  1 do
		if  newInventory[mitBooks, i1].tval = 91 then
			spell_flag:= spell_flag or newInventory[mitBooks, i1].flags;

	i1 := 0;
	while (spell_flag > 0) do
		begin
		i2 := bit_pos(spell_flag);
		with priest_spell[py.misc.pclass,i2] do
			if (slevel <= py.misc.lev) then
				if (not(learned)) then
					begin
					i1 := i1 + 1;
					test_array[i1] := i2;
					end;
		end;

	case wis_adj of
		0 :
			i2 := 0;
		1 :
			i2 := 1;
		2 :
			i2 := 1;
		3 :
			i2 := 1;
		4 :
			i2 := randint(2);
		5 :
			i2 := randint(2);
		6 :
			i2 := randint(3);
		7 :
			i2 := randint(2)+1;
		end;

	new_spell := 0;
	while ((i1 > 0) and (i2 > 0)) do
		begin
		i3 := randint(i1);
		priest_spell[py.misc.pclass,test_array[i3]].learned := true;
		new_spell := new_spell + 1;
		for i4 := i3 to i1-1 do
			test_array[i4] := test_array[i4+1];

		i1 := i1 - 1;       { One less spell to learn   }
		i2 := i2 - 1;       { Learned one   }
		end;

	if (new_spell > 0) then
		begin
		if (new_spell > 1) then
			msg_print('You learned new prayers!')
		else
			msg_print('You learned a new prayer!');

		if (py.misc.exp = 0) then msg_print(' ');
			if (py.misc.mana = 0) then
				begin
				py.misc.mana  := 1;
				py.misc.cmana := 1;
				end;

		learn_prayer := true;
		end
	else
		learn_prayer := false;
	end;


{ Gain some mana if you know at least one spell    }
//[psect(misc2$code)]
procedure gain_mana(amount: Integer);
	var
	i1,
	new_mana: Integer;
	knows_spell: Boolean;

	begin
	knows_spell:= False;
	for i1:= 1 to 31 do
		if (mage_spell[py.misc.pclass, i1].learned) then
			knows_spell:= True;
	for i1:= 1 to 31 do
		if (priest_spell[py.misc.pclass, i1].learned) then
			knows_spell:= True;
	for i1:= 1 to 31 do
		if (extra_spell[py.misc.pclass, i1].learned) then
			knows_spell:= True;
	if (knows_spell) then
		begin
		if (odd(py.misc.lev)) then
			case amount of
				0:
					new_mana:= 0;
				1:
					new_mana:= 1;
				2:
					new_mana:= 1;
				3:
					new_mana:= 1;
				4:
					new_mana:= 2;
				5:
					new_mana:= 2;
				6:
					new_mana:= 3;
				7:
					new_mana:= 4;
				else
					new_mana:= 0;
				end
		else
			case amount of
				0:
					new_mana:= 0;
				1:
					new_mana:= 1;
				2:
					new_mana:= 1;
				3:
					new_mana:= 2;
				4:
					new_mana:= 2;
				5:
					new_mana:= 3;
				6:
					new_mana:= 3;
				7:
					new_mana:= 4;
				else
					new_mana:= 0;
				end;

		py.misc.mana := py.misc.mana + new_mana;
		py.misc.cmana:= py.misc.cmana + new_mana;
		end;
	end;


{ Increases hit points and level    }
//[psect(misc2$code)]
procedure gain_level;
	var
	nhp,
	dif_exp,
	need_exp: Integer;
	redraw: Boolean;
	out_val: VType;

	begin
	with py.misc do
		begin
		nhp     := get_hitdie;
		mhp     := mhp + nhp;
		chp     := chp + nhp;
		lev     := lev + 1;
		need_exp:= Trunc(player_exp[lev] * expfact);
		if (py.misc.exp > need_exp) then
			begin
			dif_exp    := py.misc.exp - need_exp;
			py.misc.exp:= need_exp + (dif_exp div 2);
			end;
		title:= player_title[pclass, lev];
		out_val:= ShortString('Welcome to level ' + IntToStr(lev) + '.');
		msg_print(out_val);
		msg_print(' ');
		msg_flag:= False;
		prt_mhp;
		prt_chp;
		prt_level;
		prt_title;
		with class_[pclass] do
			begin
			if (mspell) then
				begin
				redraw:= False;
				learn_spell(redraw);
				if (redraw) then
					draw_cave;
				gain_mana(int_adj);
				prt_mana;
				end
			else if (pspell) then
				begin
				learn_prayer;
				gain_mana(wis_adj);
				prt_mana;
				end;
			if (espell) then
				begin
				redraw:= False;
				learn_extra(redraw);
				if (redraw) then
					draw_cave;
				gain_mana(wis_adj);
				prt_mana;
				end;
			end;
		end;
	end;


{ Prints experience }
procedure prt_experience;
//	var
//	tmp_exp: Integer;

	begin
	with py.misc do
		begin
		if (exp > player_max_exp) then
			exp:= player_max_exp;
		if (lev < max_player_level) then
			begin
			while (Trunc(player_exp[lev] * expfact) <= exp) do
				gain_level;
			if (exp > max_exp) then
				max_exp:= exp;
			end;
		end;
	prt_num('', py.misc.exp, 15, stat_column + 6);
	end;


{ Inserts a string into a string    }
procedure insert_str(var object_str: AnsiString; mtc_str, insert_str: AnsiString); overload;
	begin
{$IFDEF DCC}
	object_str:= System.AnsiStrings.StringReplace(object_str, mtc_str, insert_str, [rfIgnoreCase]);
{$ELSE}
	object_str:= AnsiReplaceStr(object_str, mtc_str, insert_str);
{$ENDIF}
	end;

procedure insert_str(var object_str: TType; mtc_str: AnsiString;
		ainsert_str: ShortString); overload;
	var
	s1,
	s2: AnsiString;

	begin
	s1:= object_str;
	s2:= ainsert_str;
	insert_str(s1, mtc_str, s2);
	object_str:= s1;
	ainsert_str:= s2;
	end;

procedure insert_str(var object_str: VType; mtc_str, ainsert_str: AnsiString); overload;
	var
	s: AnsiString;

	begin
	s:= object_str;
	insert_str(s, mtc_str, ainsert_str);
	object_str:= s;
	end;

{ Inserts a number into a string    }
//[psect(misc1$code)]
procedure insert_num(var object_str: AnsiString; mtc_str: AnsiString;
		number: Integer; show_sign: boolean); overload;
	var
	pos0, olen, mlen: Integer;
	str1, str2:      VType;

	begin
	pos0:= pos(mtc_str, object_str);
	if (pos0 > 0) then
		begin
		olen      := length(object_str);
		mlen      := length(mtc_str);
		object_str:= object_str + ' ';
		str1      := copy(object_str, 1, pos0 - 1);
		str2      := copy(object_str, pos0 + mlen,
			olen - (pos0 + mlen - 1));
		if ((number >= 0) and (show_sign)) then
			object_str:= ShortString(string(str1) + '+' + string(IntToStr(number)) + string(str2))
		else
			object_str:= ShortString(string(str1) + IntToStr(number) + string(str2))
		end
	end;

procedure insert_num(var object_str: VType; mtc_str: AnsiString;
		number: Integer; show_sign: boolean); overload; inline;
	var
	s: AnsiString;

	begin
	s:= object_str;
	insert_num(s, mtc_str, number, show_sign);
	object_str:= s;
	end;

procedure insert_num(var object_str: TType; mtc_str: AnsiString;
		number: Integer; show_sign: boolean); overload; inline;
	var
	s: AnsiString;

	begin
	s:= object_str;
	insert_num(s, mtc_str, number, show_sign);
	object_str:= s;
	end;


{ Weapon weight VS strength and dexterity   }
//[psect(moria$code)]
function attack_blows(weight: Integer; var wtohit: Integer): Integer;
	var
		adj_weight, blows: Integer;
	begin
	blows := 1;
	wtohit:= 0;
	with py.stat do
		begin
		if ((cstr * 15) < weight) then
			wtohit:= -weight
		else
			begin
			if (cdex < 10) then
				blows:= 1
			else if (cdex < 19) then
				blows:= 2
			else if (cdex < 68) then
				blows:= 3
			else if (cdex < 108) then
				blows:= 4
			else if (cdex < 118) then
				blows:= 4
			else
				blows := 5;

			adj_weight:= ((cstr * 10) div weight);
			if (adj_weight < 2) then
				blows:= 1
			else if (adj_weight < 3) then
				blows:= Trunc(blows / 3.0)
			else if (adj_weight < 4) then
				blows:= Trunc(blows / 2.5)
			else if (adj_weight < 5) then
				blows:= Trunc(blows / 2.25)
			else if (adj_weight < 7) then
				blows:= Trunc(blows / 2.00)
			else if (adj_weight < 9) then
				blows:= Trunc(blows / 1.75)
			else
				blows:= Trunc(blows / 1.50);
			end;
		end;

	attack_blows:= blows;
	end;


{ Critical hits, Nasty way to die...    }
//[psect(moria$code)]
function critical_blow(weight, plus, dam: Integer; vun: Boolean): Integer;
	begin
	critical_blow:= dam;
{ Weight of weapon, pluses to hit, and character level all
contribute to the chance of a critical }
	if  vun
	or  (randint(5000) <= (weight + 5 * plus + 3 * py.misc.lev)) then
		begin
		weight:= weight + randint(650);
		if (weight < 400) then
			begin
			critical_blow:= 2 * dam + 5;
			msg_print('It was a good hit! (x2 damage)');
			end
		else if (weight < 700) then
			begin
			critical_blow:= 3 * dam + 10;
			msg_print('It was an excellent hit! (x3 damage)');
			end
		else if (weight < 900) then
			begin
			critical_blow:= 4 * dam + 15;
			msg_print('It was a superb hit! (x4 damage)');
			end
		else
			begin
			critical_blow:= 5 * dam + 20;
			msg_print('It was a *GREAT* hit! (x5 damage)');
			end
		end;
	end;


{ Given direction 'dir', returns new row, column location   }
//[psect(misc1$code)]
function Move(dir: Integer; var y, x: Integer): boolean;
	var
	new_row,
	new_col: Integer;

	begin
	new_row:= 0;
	new_col:= 0;

	case dir of
		1:
			begin
			new_row:= y + 1;
			new_col:= x - 1;
			end;
		2:
			begin
			new_row:= y + 1;
			new_col:= x;
			end;
		3:
			begin
			new_row:= y + 1;
			new_col:= x + 1;
			end;
		4:
			begin
			new_row:= y;
			new_col:= x - 1;
			end;
		5:
			begin
			new_row:= y;
			new_col:= x;
			end;
		6:
			begin
			new_row:= y;
			new_col:= x + 1;
			end;
		7:
			begin
			new_row:= y - 1;
			new_col:= x - 1;
			end;
		8:
			begin
			new_row:= y - 1;
			new_col:= x;
			end;
		9:
			begin
			new_row:= y - 1;
			new_col:= x + 1;
			end;
	end;
	Move:= False;
	if ((new_row >= 1) and (new_row <= cur_height)) then
		if ((new_col >= 1) and (new_col <= cur_width)) then
			begin
			y   := new_row;
			x   := new_col;
			Move:= True;
			end
	end;


{ Saving throws for player character... }
//[psect(moria$code)]
function player_saves(adjust : integer) : boolean;
	begin
	if (randint(100) <= (py.misc.save + adjust)) then
		player_saves := true
	else
		player_saves := false;
	end;


procedure Opusii_vomit(percentage: Integer);
	begin
	if (randint(100) < percentage) then
		with py.flags do
			begin
			food     := food - 500;
			paralysis:= 4;
			msg_print('You feel your insides heave!');
			end
	end;

procedure TopTwentyItmCallback(const AListView: TScrListView;
		const AScreen: TCustomSimpleScreen; const APosition: TScreenPos;
		const ATab: Integer; const APage: Integer; const AItem: Integer;
		const AData: Pointer);
	type
	PText = ^Text;

	var
	f1: PText;
	l: VType;

	begin
	f1:= PText(AData);
	if not Eof(f1^) then
		Readln(f1^, l)
	else
		l:= '';

	screen.Writeln(l);
	end;

procedure DisplayTopTwenty;
	var
	f1: Text;
//	n1: Integer;
//	l: VType;
	dummy: AnsiChar;
	lst: TScrListView;
	p: TScreenPos;

	begin
	clear(1, 1);

//	prt('- M O R I A   T O P   T W E N T Y -', STR_CLR_NMHHLT, 1, 22);
//	screen.Writeln;
//	screen.writeln('Username       Points  Character that died.');

	AssignFile(f1, string(moria_top));
	Reset(f1);

//	n1:= 1;

	if IOResult = 0 then
		begin
		lst:= TScrListView.Create(screen, nil, TopTwentyItmCallback,
				Pointer(@f1));
		try
			p.X:= 1;
			p.Y:= 3;
			lst.Position:= p;
			lst.Title:= '                      - M O R I A   T O P   T W E N T Y -';
			lst.Heading:= 'Username       Points  Character that died.';
			lst.PageSize:= 20;
			lst.ItemCount:= 20;
			lst.ShowCurrent:= False;
			lst.Display;

(*			while ((not eof(f1)) and (n1 <= 20)) do
				begin
				Readln(f1, l);
				n1 := n1 + 1;

				screen.Writeln(l);
				end;*)
			finally
			lst.Free;
			end;

		CloseFile(f1);
		end;

	prt('[Press any key to continue]', STR_CLR_NMHHLT, 24, 26);
	inkey(dummy);
	end;

procedure DecodeClrTextToScreen(ALine: VType; var ARow: Integer);
	var
	cs: AnsiString;
	cc: AnsiChar;
	i: Integer;

	begin
	cs:= '';
	for i:= 1 to Length(ALine) do
		begin
		cc:= COLR_NORMAL;

		case ALine[i] of
			'b':
				cc:= AnsiChar(attrFGBlue);
			'B':
				cc:= AnsiChar(attrFGLightBlue);
			'c':
				cc:= AnsiChar(attrFGCyan);
			'C':
				cc:= AnsiChar(attrFGLightCyan);
			'g':
				cc:= AnsiChar(attrFGGreen);
			'G':
				cc:= AnsiChar(attrFGLightGreen);
			'k':
				cc:= AnsiChar(attrFGBlack);
			'K':
				cc:= AnsiChar(attrFGDarkGray);
			'm':
				cc:= AnsiChar(attrFGMagenta);
			'M':
				cc:= AnsiChar(attrFGLightMagenta);
			'r':
				cc:= AnsiChar(attrFGRed);
			'R':
				cc:= AnsiChar(attrFGLightRed);
			'w':
				cc:= AnsiChar(attrFGLightGray);
			'W':
				cc:= AnsiChar(attrFGWhite);
			'y':
				cc:= AnsiChar(attrFGBrown);
			'Y':
				cc:= AnsiChar(attrFGYellow);
			end;

		cs:= cs + cc;
		end;

	PutColour(cs, ARow, 1);
	Inc(ARow);
	end;

{ Centers a string within a 31 character string }
function fill_str(p1: vtype): vtype;
	var
	s1: vtype;
	i1: integer;
	begin
	s1      := '';
	i1      := Length(p1) div 2;
//		fill_str:= substr(pad(s1, ' ', 15 - i1) + pad(p1, ' ', 31), 1, 31);
	fill_str:= Copy(PadRight(s1, 15 - i1) + PadRight(p1, 31), 1, 31);
	end;


{ Prints a line to the screen efficiently }
//todo Remove - replace with normal prt.
procedure dprint(str: vtype; row: integer);
	var
	i1, i2, nblanks, xpos: integer;
	prt_str:               vtype;

	begin
	prt_str:= '';
	nblanks:= 0;
	xpos   := 0;
	for i1 := 1 to Length(str) do
		begin
		if (str[i1] = ' ') then
			begin
			if (xpos > 0) then
				begin
				nblanks:= nblanks + 1;
				if (nblanks > 5) then
					begin
					nblanks:= 0;
					put_buffer(prt_str, STR_CLR_NORMAL, row, xpos);
					prt_str:= '';
					xpos   := 0;
					end
				end;
			end
		else
			begin
			if (xpos = 0) then
				xpos:= i1;
			if (nblanks > 0) then
				begin
				for i2     := 1 to nblanks do
					prt_str:= prt_str + ' ';
				nblanks    := 0;
				end;
			prt_str:= prt_str + str[i1];
			end;
		end;
	if (xpos > 0) then
		put_buffer(prt_str, STR_CLR_NORMAL, row, xpos);
	end;


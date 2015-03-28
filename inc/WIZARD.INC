{ Print Moria credits }
//[psect(misc1$code)]
procedure game_version;
	var
	tmp_str: vtype;

	begin
	clear(1, 1);
//	writev(tmp_str, '               Moria Version ', cur_version: 3: 2);
	tmp_str:= Format(
			AnsiString('                       Moria Version %d.%2.2d.%2.2d.%4.4d %s'),
			[CUR_VERSION_MAJ, CUR_VERSION_MIN, CUR_VERSION_REV, CUR_VERSION_BLD,
			CUR_VERSION_DAT]);
	put_buffer(tmp_str, STR_CLR_NMHHLT, 1, 1);
	put_buffer('Version 0.10  : 25MAR1983', STR_CLR_NORMAL, 2, 1);
	put_buffer('Version 1.00  : 01MAY1984', STR_CLR_NORMAL, 3, 1);
	put_buffer('Version 2.00  : 10JUL1984', STR_CLR_NORMAL, 4, 1);
	put_buffer('Version 3.00  : 20NOV1984', STR_CLR_NORMAL, 5, 1);
	put_buffer('Version 4.00  : 20JAN1985', STR_CLR_NORMAL, 6, 1);
	put_buffer('Version 5.01  : 10OCT2012', STR_CLR_NORMAL, 7, 1);

	put_buffer('Modules :', STR_CLR_NMHHLT, 9, 1);
	put_buffer('     V1.00  Dungeon Generator      - RAK', STR_CLR_NORMAL, 10, 1);
	put_buffer('            Character Generator    - RAK & JWT', STR_CLR_NORMAL, 11, 1);
	put_buffer('            Moria Module           - RAK', STR_CLR_NORMAL, 12, 1);
	put_buffer('            Miscellaneous          - RAK & JWT', STR_CLR_NORMAL, 13, 1);
	put_buffer('     V2.00  Town Level & Misc      - RAK', STR_CLR_NORMAL, 14, 1);
	put_buffer('     V3.00  Internal Help & Misc   - RAK', STR_CLR_NORMAL, 15, 1);
	put_buffer('     V4.00  Source Release Version - RAK', STR_CLR_NORMAL, 16, 1);
	put_buffer('     V5.01  Port to Delphi/FP      - DENGLAND', STR_CLR_NORMAL, 17, 1);
	put_buffer('Robert Alan Koeneke               Jimmey Wayne Todd Jr.', STR_CLR_NORMAL, 19, 1);
	put_buffer('Student/University of Oklahoma    Student/University of Oklahoma', STR_CLR_NORMAL, 20, 1);
	put_buffer('119 Crystal Bend                  1912 Tiffany Dr.', STR_CLR_NORMAL, 21, 1);
	put_buffer('Norman, OK 73069                  Norman, OK  73071', STR_CLR_NORMAL, 22, 1);
	put_buffer('(405)-321-2925                    (405) 360-6792', STR_CLR_NORMAL, 23, 1);
	pause(24);
	draw_cave;
	end;


		{ Light up the dungeon                                  -RAK-   }
//[psect(wizard$code)]
procedure wizard_light;
	var
	i1, i2, i3, i4: integer;
	flag:           boolean;

	begin
	if (cave[char_row, char_col].pl) then
		flag:= false
	else
		flag  := true;
	for i1    := 1 to cur_height do
		for i2:= 1 to cur_width do
			if (cave[i1, i2].fval in floor_set) then
				for i3    := i1 - 1 to i1 + 1 do
					for i4:= i2 - 1 to i2 + 1 do
						with cave[i3, i4] do
							begin
							pl:= flag;
							if (not(flag)) then
								fm:= false;
							end;
	prt_map;
	end;



{ Wizard routine for gaining on stats }
//[psect(wizard$code)]
procedure change_character;
	var
	tmp_val: integer;
	tmp_str: vtype;

	begin
	with py.stat do
		begin
		prt('(3 - 118) Strength     = ', 1, 1);
		get_string(tmp_str, 1, 26, 10);
//		tmp_val:= -999;
//		readv(tmp_str, tmp_val, error:= continue);
		if not TryStrToInt(string(tmp_str), tmp_val) then
			tmp_val:= -999;

		if ((tmp_val > 2) and (tmp_val < 119)) then
			begin
			str := tmp_val;
			cstr:= tmp_val;
			prt_strength;
			prt_max_weight;
			end;
		prt('(3 - 118) Intelligence = ', 1, 1);
		get_string(tmp_str, 1, 26, 10);
//		tmp_val:= -999;
//		readv(tmp_str, tmp_val, error:= continue);
		if not TryStrToInt(string(tmp_str), tmp_val) then
			tmp_val:= -999;
		if ((tmp_val > 2) and (tmp_val < 119)) then
			begin
			int := tmp_val;
			cint:= tmp_val;
			prt_intelligence;
			end;
		prt('(3 - 118) Wisdom       = ', 1, 1);
		get_string(tmp_str, 1, 26, 10);
//		tmp_val:= -999;
//		readv(tmp_str, tmp_val, error:= continue);
		if not TryStrToInt(string(tmp_str), tmp_val) then
			tmp_val:= -999;
		if ((tmp_val > 2) and (tmp_val < 119)) then
			begin
			wis := tmp_val;
			cwis:= tmp_val;
			prt_wisdom;
			end;
		prt('(3 - 118) Dexterity    = ', 1, 1);
		get_string(tmp_str, 1, 26, 10);
//		tmp_val:= -999;
//		readv(tmp_str, tmp_val, error:= continue);
		if not TryStrToInt(string(tmp_str), tmp_val) then
			tmp_val:= -999;
		if ((tmp_val > 2) and (tmp_val < 119)) then
			begin
			dex := tmp_val;
			cdex:= tmp_val;
			prt_dexterity;
			end;
		prt('(3 - 118) Constitution = ', 1, 1);
		get_string(tmp_str, 1, 26, 10);
//		tmp_val:= -999;
//		readv(tmp_str, tmp_val, error:= continue);
		if not TryStrToInt(string(tmp_str), tmp_val) then
			tmp_val:= -999;
		if ((tmp_val > 2) and (tmp_val < 119)) then
			begin
			con := tmp_val;
			ccon:= tmp_val;
			prt_constitution;
			end;
		prt('(3 - 118) Charisma     = ', 1, 1);
		get_string(tmp_str, 1, 26, 10);
//		tmp_val:= -999;
//		readv(tmp_str, tmp_val, error:= continue);
		if not TryStrToInt(string(tmp_str), tmp_val) then
			tmp_val:= -999;
		if ((tmp_val > 2) and (tmp_val < 119)) then
			begin
			chr := tmp_val;
			cchr:= tmp_val;
			prt_charisma;
			end;
		end;
	with py.misc do
		begin
		prt('(1 - 32767) Hit points = ', 1, 1);
		get_string(tmp_str, 1, 26, 10);
//		tmp_val:= -1;
//		readv(tmp_str, tmp_val, error:= continue);
		if not TryStrToInt(string(tmp_str), tmp_val) then
			tmp_val:= -1;
		if ((tmp_val > 0) and (tmp_val < 32768)) then
			begin
			mhp:= tmp_val;
			chp:= tmp_val;
			prt_mhp;
			prt_chp;
			end;
		if ((py.misc.tclass <> 'Rogue') and (py.misc.tclass <> 'Warrior')) then
			begin
			prt('(0 - 32767) Mana       = ', 1, 1);
			get_string(tmp_str, 1, 26, 10);
//			tmp_val:= -999;
//			readv(tmp_str, tmp_val, error:= continue);
			if not TryStrToInt(string(tmp_str), tmp_val) then
				tmp_val:= -999;
			if ((tmp_val > -1) and (tmp_val < 32768)) then
				begin
				mana := tmp_val;
				cmana:= tmp_val;
				prt_mana;
				end;
			end;

//		writev(tmp_str, 'Current=', srh: 1, '  (0-200) Searching = ');
		tmp_str:= Format(
				AnsiString('Current=%d  (0-200) Searching = '), [srh]);
		tmp_val:= length(tmp_str);
		prt(tmp_str, 1, 1);
		get_string(tmp_str, 1, tmp_val + 1, 10);
//		tmp_val:= -999;
//		readv(tmp_str, tmp_val, error:= continue);
		if not TryStrToInt(string(tmp_str), tmp_val) then
			tmp_val:= -999;
		if ((tmp_val > -1) and (tmp_val < 201)) then
			srh:= tmp_val;

//		writev(tmp_str, 'Current=', stl: 1, '  (0-10) Stealth = ');
		tmp_str:= Format(
				AnsiString('Current=%d  (0-10) Stealth = '), [stl]);
		tmp_val:= length(tmp_str);
		prt(tmp_str, 1, 1);
		get_string(tmp_str, 1, tmp_val + 1, 10);
//		tmp_val:= -999;
//		readv(tmp_str, tmp_val, error:= continue);
		if not TryStrToInt(string(tmp_str), tmp_val) then
			tmp_val:= -999;
		if ((tmp_val > -1) and (tmp_val < 11)) then
			stl:= tmp_val;

//		writev(tmp_str, 'Current=', disarm: 1, '  (0-200) Disarming = ');
		tmp_str:= Format(
				AnsiString('Current=%d  (0-200) Disarming = '), [disarm]);
		tmp_val:= length(tmp_str);
		prt(tmp_str, 1, 1);
		get_string(tmp_str, 1, tmp_val + 1, 10);
//		tmp_val:= -999;
//		readv(tmp_str, tmp_val, error:= continue);
		if not TryStrToInt(string(tmp_str), tmp_val) then
			tmp_val:= -999;
		if ((tmp_val > -1) and (tmp_val < 201)) then
			disarm:= tmp_val;

//		writev(tmp_str, 'Current=', save: 1, '  (0-100) Save = ');
		tmp_str:= Format(
				AnsiString('Current=%d  (0-100) Save = '), [save]);
		tmp_val:= length(tmp_str);
		prt(tmp_str, 1, 1);
		get_string(tmp_str, 1, tmp_val + 1, 10);
//		tmp_val:= -999;
//		readv(tmp_str, tmp_val, error:= continue);
		if not TryStrToInt(string(tmp_str), tmp_val) then
			tmp_val:= -999;
		if ((tmp_val > -1) and (tmp_val < 201)) then
			save:= tmp_val;

//		writev(tmp_str, 'Current=', bth: 1, '  (0-200) Base to hit = ');
		tmp_str:= Format(
				AnsiString('Current=%d  (0-200) Base to Hit = '), [bth]);
		tmp_val:= length(tmp_str);
		prt(tmp_str, 1, 1);
		get_string(tmp_str, 1, tmp_val + 1, 10);
//		tmp_val:= -999;
//		readv(tmp_str, tmp_val, error:= continue);
		if not TryStrToInt(string(tmp_str), tmp_val) then
			tmp_val:= -999;
		if ((tmp_val > -1) and (tmp_val < 201)) then
			bth:= tmp_val;

//		writev(tmp_str, 'Current=', bthb: 1, '  (0-200) Bows/Throwing = ');
		tmp_str:= Format(
				AnsiString('Current=%d  (0-200) Bows/Throwing = '), [bthb]);
		tmp_val:= length(tmp_str);
		prt(tmp_str, 1, 1);
		get_string(tmp_str, 1, tmp_val + 1, 10);
//		tmp_val:= -999;
//		readv(tmp_str, tmp_val, error:= continue);
		if not TryStrToInt(string(tmp_str), tmp_val) then
			tmp_val:= -999;
		if ((tmp_val > -1) and (tmp_val < 201)) then
			bthb:= tmp_val;

//		writev(tmp_str, 'Current=', au: 1, '  Gold = ');
		tmp_str:= Format(
				AnsiString('Current=%d  Gold = '), [au]);
		tmp_val:= length(tmp_str);
		prt(tmp_str, 1, 1);
		get_string(tmp_str, 1, tmp_val + 1, 10);
//		tmp_val:= -999;
//		readv(tmp_str, tmp_val, error:= continue);
		if not TryStrToInt(string(tmp_str), tmp_val) then
			tmp_val:= -999;
		if (tmp_val > -1) then
			begin
			au:= tmp_val;
			prt_gold;
			end;
		end;
	erase_line(msg_line, msg_line);
//	py_bonuses(blank_treasure, 0);
	ApplyBonuses(@blank_treasure, 0);
	end;

		{ Wizard routine for creating objects                   -RAK-   }
//[psect(wizard$code)]
procedure wizard_create;
	var
	tmp_val: integer;
	tmp_str: vtype;
	flag:    boolean;

	begin
	msg_print('Warning: This routine can cause fatal error.');
	msg_print(' ');
	msg_flag:= false;
	with invenTemp do
		begin
		prt('Name   : ', 1, 1);
		if (get_string(tmp_str, 1, 10, 60)) then
			name:= tmp_str
		else
			name:= '& Wizard Object!';
		repeat
			prt('Tval   : ', 1, 1);
			get_string(tmp_str, 1, 10, 10);
//			tmp_val:= 0;
//			readv(tmp_str, tmp_val, error:= continue);
			if not TryStrToInt(string(tmp_str), tmp_val) then
				tmp_val:= 0;
			flag:= true;
			case tmp_val of
				1, 13, 15:
					tchar:= '~';
				2:
					tchar:= '&';
				10:
					tchar:= '{';
				11:
					tchar:= '{';
				12:
					tchar:= '{';
				20:
					tchar:= '}';
				21:
					tchar:= '/';
				22:
					tchar:= '\';
				23:
					tchar:= '|';
				25:
					tchar:= '\';
				30:
					tchar:= ']';
				31:
					tchar:= ']';
				32:
					tchar:= '(';
				33:
					tchar:= ']';
				34:
					tchar:= ')';
				35:
					tchar:= '[';
				36:
					tchar:= '(';
				40:
					tchar:= '"';
				45:
					tchar:= '=';
				55:
					tchar:= '_';
				60:
					tchar:= '-';
				65:
					tchar:= '-';
				70, 71:
					tchar:= '?';
				75, 76, 77:
					tchar:= '!';
				80:
					tchar:= ',';
				90:
					tchar:= '?';
				91:
					tchar:= '?';
				else
					flag:= false;
			end;
			until (flag);

		tval:= tmp_val;
		prt('Subval : ', 1, 1);
		get_string(tmp_str, 1, 10, 10);
//		tmp_val:= 1;
//		readv(tmp_str, tmp_val, error:= continue);
		if not TryStrToInt(string(tmp_str), tmp_val) then
			tmp_val:= 1;
		subval:= tmp_val;
		prt('Weight : ', 1, 1);
		get_string(tmp_str, 1, 10, 10);
//		tmp_val:= 1;
//		readv(tmp_str, tmp_val, error:= continue);
		if not TryStrToInt(string(tmp_str), tmp_val) then
			tmp_val:= 1;
		weight:= tmp_val;
		prt('Number : ', 1, 1);
		get_string(tmp_str, 1, 10, 10);
//		tmp_val:= 1;
//		readv(tmp_str, tmp_val, error:= continue);
		if not TryStrToInt(string(tmp_str), tmp_val) then
			tmp_val:= 1;
		number:= tmp_val;
		prt('Damage : ', 1, 1);
		get_string(tmp_str, 1, 10, 5);
		damage:= tmp_str;
		prt('+To hit: ', 1, 1);
		get_string(tmp_str, 1, 10, 10);
//		tmp_val:= 0;
//		readv(tmp_str, tmp_val, error:= continue);
		if not TryStrToInt(string(tmp_str), tmp_val) then
			tmp_val:= 0;
		tohit:= tmp_val;
		prt('+To dam: ', 1, 1);
		get_string(tmp_str, 1, 10, 10);
//		tmp_val:= 0;
//		readv(tmp_str, tmp_val, error:= continue);
		if not TryStrToInt(string(tmp_str), tmp_val) then
			tmp_val:= 0;
		todam:= tmp_val;
		prt('AC     : ', 1, 1);
		get_string(tmp_str, 1, 10, 10);
//		tmp_val:= 0;
//		readv(tmp_str, tmp_val, error:= continue);
		if not TryStrToInt(string(tmp_str), tmp_val) then
			tmp_val:= 0;
		ac:= tmp_val;
		prt('+To AC : ', 1, 1);
		get_string(tmp_str, 1, 10, 10);
//		tmp_val:= 0;
//		readv(tmp_str, tmp_val, error:= continue);
		if not TryStrToInt(string(tmp_str), tmp_val) then
			tmp_val:= 0;
		toac:= tmp_val;
		prt('P1     : ', 1, 1);
		get_string(tmp_str, 1, 10, 10);
//		tmp_val:= 0;
//		readv(tmp_str, tmp_val, error:= continue);
		if not TryStrToInt(string(tmp_str), tmp_val) then
			tmp_val:= 0;
		p1:= tmp_val;
		prt('Flags (In HEX): ', 1, 1);
		flags:= get_hex_value(1, 17, 8);
		prt('Cost : ', 1, 1);
		get_string(tmp_str, 1, 10, 10);
//		tmp_val:= 0;
//		readv(tmp_str, tmp_val, error:= continue);
		if not TryStrToInt(string(tmp_str), tmp_val) then
			tmp_val:= 0;
		cost:= tmp_val;
		if (get_com('Allocate? (Y/N)', command)) then
			case command of
				'y', 'Y':
					begin
					popt(tmp_val);
					t_list[tmp_val]:= invenTemp;
					with cave[char_row, char_col] do
						begin
						if (tptr > 0) then
							delete_object(char_row, char_col);
						tptr:= tmp_val;
						end;
					msg_print('Allocated...');
					end;
				else
					msg_print('Aborted...');
				end;
		invenTemp:= blank_treasure;
		end;
	end;


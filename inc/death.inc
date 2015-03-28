{ Handles the gravestone and top-twenty routines }

{ Change the player into a King! }
procedure kingly;
	var
	i: Integer;

	begin
{ Change the character attributes.}
	dun_level := 0;
	died_from := 'ripe old age';
	with py.misc do
		begin
		lev := lev + max_player_level;
		if (sex[1] = 'M') then
			begin
			title  := 'Magnificent';
			tclass := '*King*'
			end
		else
			begin
			title  := 'Beautiful';
			tclass := '*Queen*'
			end;
		au := au + 250000;
		max_exp := max_exp + 5000000;
		exp := max_exp
		end;
{ Let the player know that he did good.}
	clear(1,1);

	screen.BeginUpdate;
	try
		dprint('                                  #',2);
		dprint('                                #####',3);
		dprint('                                  #',4);
		dprint('                            ,,,  $$$  ,,,',5);
		dprint('                        ,,=$   "$$$$$"   $=,,',6);
		dprint('                      ,$$        $$$        $$,',7);
		dprint('                      *>         <*>         <*',8);
		dprint('                      $$         $$$         $$',9);
		dprint('                      "$$        $$$        $$"',10);
		dprint('                       "$$       $$$       $$"',11);
		dprint('                        *#########*#########*',12);
		dprint('                        *#########*#########*',13);


		dprint('                          Veni, Vidi, Vici!',16);
		dprint('                     I came, I saw, I conquered!',17);
		dprint('                      All Hail the Mighty King!',18);

		i:= 2;
		DecodeClrTextToScreen('                                  R',i);
		DecodeClrTextToScreen('                                RRRRR',i);
		DecodeClrTextToScreen('                                  R',i);
		DecodeClrTextToScreen('                            YYY  YYY  YYY',i);
		DecodeClrTextToScreen('                        YYYY   YYYYYYY   YYYY',i);
		DecodeClrTextToScreen('                      YYY        YYY        YYY',i);
		DecodeClrTextToScreen('                      GY         YGY         YG',i);
		DecodeClrTextToScreen('                      YY         YYY         YY',i);
		DecodeClrTextToScreen('                      YYY        YYY        YYY',i);
		DecodeClrTextToScreen('                       YYY       YYY       YYY',i);
		DecodeClrTextToScreen('                        YmmmmmmmmmYmmmmmmmmmY',i);
		DecodeClrTextToScreen('                        YmmmmmmmmmYmmmmmmmmmY',i);

		i:= 16;
		DecodeClrTextToScreen('                          wwwww wwwww wwwww',i);
		DecodeClrTextToScreen('                     w wwwww w wwww w wwwwwwwwww',i);
		DecodeClrTextToScreen('                      WWW WWWW WWW WWWWWW WWWWW',i);

		finally
		screen.EndUpdate;
		end;

	flush;
	pause(24);
	end;

{ Prints the gravestone of the character }
procedure print_tomb;
	var
	str1,
	str2,
	str3,
	str4,
	str5,
	str6,
	str7,
	str8: VType;
	dstr: array [0 .. 19] of VType;
	fnam:    VType;
	command: AnsiChar;
	f1:      Text;
	i,
	i1: Integer;
//	day:     packed array [1 .. 11] of char;
	day: AnsiString;
	flag:    Boolean;

	begin
//		date(day);
	day:= AnsiString(formatdatetime('ddMMMyyyy', Now));
	str1:= fill_str(py.misc.name);
	str2:= fill_str(py.misc.title);
	str3:= fill_str(py.misc.tclass);
//		writev(str4, 'Level : ', py.misc.lev: 1);
	str4:= ShortString('Level : ' + IntToStr(py.misc.lev));
	str4:= fill_str(str4);
//		writev(str5, py.misc.exp: 1, ' Exp');
	str5:= ShortString(IntToStr(py.misc.exp) + ' Exp');
	str5:= fill_str(str5);
//		writev(str6, py.misc.au: 1, ' Au');
	str6:= ShortString(IntTostr(py.misc.au) + ' Au');
	str6:= fill_str(str6);
//		writev(str7, 'Died on Level : ', dun_level: 1);
	str7:= ShortString('Died on level : ' + IntToStr(dun_level));
	str7    := fill_str(str7);
	str8    := fill_str(died_from);

	dstr[00]:= ' ';
	dstr[01]:= '               _______________________';
	dstr[02]:= '              /                       \         ___';
	dstr[03]:= '             /                         \ ___   /   \      ___';
	dstr[04]:= '            /            RIP            \   \  :   :     /   \';
	dstr[05]:= '           /                             \  : _;,,,;_    :   :';
	dstr[06]:= '          /' + str1 +                    '\,;_          _;,,,;_';
	dstr[07]:= '         |               the               |   ___';
	dstr[08]:= '         | ' + str2 +                    ' |  /   \';
	dstr[09]:= '         |                                 |  :   :';
	dstr[10]:= '         | ' + str3 +                    ' | _;,,,;_   ____';
	dstr[11]:= '         | ' + str4 +                    ' |          /    \';
	dstr[12]:= '         | ' + str5 +                    ' |          :    :';
	dstr[13]:= '         | ' + str6 +                    ' |          :    :';
	dstr[14]:= '         | ' + str7 +                    ' |         _;,,,,;_';
	dstr[15]:= '         |            killed by            |';
	dstr[16]:= '         | ' + str8 +                    ' |';
	dstr[17]:= '         |            ' + day+'            |';
	dstr[18]:= '        *|   *     *     *    *   *     *  | *';
	dstr[19]:= '________)/\\_)_/___(\/___(//_\)/_\//__\\(/_|_)_______';

	clear(1, 1);
	screen.BeginUpdate;
	try
		for i1:= 0 to 19 do
			dprint(dstr[i1], i1 + 1);

		dstr[00]:= ' ';
		dstr[01]:= '               wwwwwwwwwwwwwwwwwwwwwww';
		dstr[02]:= '              w                       w         KKK';
		dstr[03]:= '             w                         w KKK   K   K      KKK';
		dstr[04]:= '            w            WWW            w   K  K   K     K   K';
		dstr[05]:= '           w                             w  K gKGGGKg    K   K';
		dstr[06]:= '          wWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWwGKg          gKGGGKg';
		dstr[07]:= '         w               www               w   KKK';
		dstr[08]:= '         w WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW w  K   K';
		dstr[09]:= '         w                                 w  K   K';
		dstr[10]:= '         w WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW w gKGGGKg   KKKK';
		dstr[11]:= '         w WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW w          K    K';
		dstr[12]:= '         w WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW w          K    K';
		dstr[13]:= '         w WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW w          K    K';
		dstr[14]:= '         w WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW w         gKGGGGKg';
		dstr[15]:= '         w            wwwwww ww            w';
		dstr[16]:= '         w WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW w';
		dstr[17]:= '         w            WWWWWWWWW            w';
		dstr[18]:= '        rw   r     r     r    r   r     r  w r';
		dstr[19]:= 'ggggggggGGGGgGgGgggGGGgggGGGgGGGgGGGggGGGGgwgGggggggg';

		i:= 1;
		for i1:= 0 to 19 do
			DecodeClrTextToScreen(dstr[i1], i);

		finally
		screen.EndUpdate;
		end;

	flush;

	if (get_com('Print to file? (y/n)', command)) then
		case command of
			'y', 'Y':
				begin
				prt('Enter filename:', 1, 1);
				flag:= False;
				repeat
					if (get_string(fnam, 1, 17, 60)) then
						begin
						if (Length(fnam) = 0) then
							fnam:= 'MORIACHR.DIE';
//							Open(f1, file_name:= fnam, error:= continue);
						AssignFile(f1, string(fnam));
//							if (status(f1) <> 0) then
						if (IOResult <> 0) then
							prt('Error creating> ' + fnam, 2, 1)
						else
							begin
							Flag:= True;
//								Rewrite(f1, error:= continue);
							Rewrite(f1);
							for i1:= 0 to 19 do
//									Writeln(f1, dstr[i1], error:= continue);
								Writeln(f1, dstr[i1]);
							end;
//							Close(f1, error:= continue);
						CloseFile(f1);
						end
					else
						Flag:= True;
					until (flag)
				end;
			else
			end
	end;


//[psect(death$code)]
procedure upon_death;
(*	type
	word    = 0..65535;
	recj    = record
		unameinfo       : packed record
			unamelen        : word;
			jpi$_username   : word;
			end;
		ptr_uname       : ^usernam;
		ptr_unamelen    : ^integer;
		endlist         : integer
		end;
	usernam         = packed array [1..12] of AnsiChar;*)


{ function returns the players USERNAME }
	function get_username: AnsiString;(*usernam;*)
		var
		player: AnsiString;
(*		var
		user : usernam;
		icode : integer;
		jpirec : recj;

{ calls GETJPI routine to return the USERNAME }
	  function sys_getjpi       (%immed p1 : integer;
								%immed  p2 : integer;
								%immed  p3 : integer;
								var itmlst : recj;
								%immed  p4 : integer;
								%immed  p5 : integer;
								%immed  p6 : integer) : integer;
		external;

		begin
		with jpirec do
			begin
			unameinfo.unamelen := 12;
			unameinfo.jpi$_username := %x202;
			new (ptr_uname);
			ptr_uname^ := '            ';
			new (ptr_unamelen);
			ptr_unamelen^ := 0;
			endlist := 0
			end;
		icode := SYS$GETJPI (0,0,0,jpirec,0,0,0);
		if not odd(icode) then
			begin
			writeln('Error in GETJPI process');
			Halt
			end;
		else
			get_username := jpirec.ptr_uname^*)
		begin
		prt('Enter your name:', 1, 1);
		get_string(player, 1, 18, 12);
		Result:= player;
		end;

{ Calculates the total number of points earned }
	function total_points: Integer;
		begin
		with py.misc do
			total_points := max_exp + (100 * py.misc.max_lev);
		end;

{ Enters a players name on the top twenty list }
	procedure UpdateTopTwenty;
		var
		list : array [1..20] of VType;
		blank : packed array [1..13] of AnsiChar;
		i1,
		i2,
		i3,
		i4,
		n1: Integer;
//		mwk5,
//		trys : Integer;
		o1,
		o2 : VType;
		f1 : Text;
		flag: Boolean;
//		file_flag  : Boolean;
		s: AnsiString;
		p: Integer;

		begin
//dengland Only check that they have cheated
		if (py.flags.status and $08000000) <> 0 then
//			Halt(0);
			raise EMoriaTerminate.Create('On death wizard mode');

		clear(1,1);
		for i1 := 1 to 20 do
			list[i1] := '';
		n1 := 1;
//		priv_switch(1);
//		trys := 0;
//		file_flag := False;
//		repeat
//			open(f1,file_name:=moria_top, organization:=sequential,history:=old,
//					sharing:=none,error:=continue);
//			if (status(f1) = 2) then
//				begin
//				trys := trys + 1;
//				if (trys > 5) then
//					file_flag := True
//				else
//					sleep(2);
//				end
//			else
//				file_flag := True;
//			until(file_flag);
		AssignFile(f1, string(moria_top));

//		if ((status(f1) <> 0) and (status(f1) <> 2)) then
//		  open (f1,file_name:=moria_top,
//				organization:=sequential,history:=new,
//				sharing:=none,error:=continue);

		Reset(f1);

//		if (status(f1) <> 0) then
		if IOResult <> 0 then
			begin
			screen.Writeln('Error in opening ',moria_top);
			screen.Writeln('Please contact local Moria Wizard.');
//			Halt(0);
			raise EMoriaTerminate.Create('Couldn''t open top twenty file');
			end;

//		reset(f1);

		while ((not eof(f1)) and (n1 <= 20)) do
			begin
//			Readln(f1,list[n1],error:=continue);
			Readln(f1,list[n1]);
			n1 := n1 + 1;
			end;
		n1 := n1 - 1;
		i1 := 1;
		i3 := total_points;
		flag:= False;
		while ((i1 <= n1) and (not flag)) do
			begin
//			readv(list[i1],blank,i4);
			System.Move(list[i1], blank[1], Length(blank));
			s:= Trim(Copy(list[i1], Length(blank) + 1, MaxInt));
			p:= Pos(AnsiString(' '), s);
			if p > 0 then
				s:= Copy(s, 1, p - 1);
			i4:= StrToInt(string(s));

			if (i4 < i3) then
			  flag := true
			else
			  i1 := i1 + 1;
			end;

		if ((flag) or (n1 = 0) or (n1 < 20)) then
			begin
			for i2 := 19 downto i1 do
				list[i2+1] := list[i2];
			o1 := get_username;
			case py.misc.lev of
				1 :
//					writev(o2,py.misc.lev:1,'st level ');
					o2:= ShortString(IntToStr(py.misc.lev) + 'st level ');
				2 :
//					writev(o2,py.misc.lev:1,'nd level ');
					o2:= ShortString(IntToStr(py.misc.lev) + 'nd level ');
				3 :
//					writev(o2,py.misc.lev:1,'rd level ');
					o2:= ShortString(IntToStr(py.misc.lev) + 'rd level ');
				else
//					writev(o2,py.misc.lev:1,'th level ')
					o2:= ShortString(IntToStr(py.misc.lev) + 'th level ');
				end;

//			writev(list[i1],pad(o1,' ',13),i3:7,'  ',
//				py.misc.name,', a ',o2,py.misc.race,' ',
//				py.misc.tclass,'.');
			list[i1]:= PadRight(o1, 13) + ' ' +
					PadLeft(AnsiString(IntToStr(i3)), 7) + '  ' +
					py.misc.name + ', a ' + o2 + py.misc.race + ' ' +
					py.misc.tclass + '.';
			if (n1 < 20) then
				n1 := n1 + 1;

//			flag := false;

			end;
		rewrite(f1);
		for i1 := 1 to n1 do
			writeln(f1,list[i1]);
		close(f1);
//		priv_switch(0);

//		clear(1, 1);
//		prt('- M O R I A   T O P   T W E N T Y -', STR_CLR_NMHHLT, 1, 22);
//		screen.Writeln;
//		screen.writeln('Username       Points  Character that died.');
//		for i1 := 1 to n1 do
//			begin
//			screen.Writeln(list[i1]);
//
////			mwk5:=i1
//
//			end;
//
////		mwk5:=mwk5+2;
//
//		screen.Writeln;
//		put_qio
		end;

	var
	dummy: AnsiChar;

{ What happens upon dying...}
	begin
	if (total_winner) then
		kingly;
	print_tomb;

//	top_forty;
	UpdateTopTwenty;
	DisplayTopTwenty;

//dengland Add this because the terminal disappears at program exit, these days.
	prt('[Press any key to exit]', STR_CLR_NMHHLT, 24, 20);
	inkey(dummy);

	screen.Writeln;
//	Halt(0);
	raise EMoriaTerminate.Create('On death');
	end;


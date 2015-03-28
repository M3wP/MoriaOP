procedure Configure;
	var
	iniFile: TMemIniFile;
	sl: TStringList;
	i, v: Integer;

	begin
	iniFile:= TMemIniFile.Create('config.ini');
	sl:= TStringList.Create;
	try
//		Read debug section settings
		iniFile.ReadSectionValues('DEBUG', sl);

//		Get ErrorLog setting
		i:= sl.IndexOfName('ErrorLog');
		if  (i = -1)
		or  (not TryStrToInt(sl.ValueFromIndex[i], v))
		or  (v = 0) then
			LogErrors:= False
		else
			begin
			LogErrors:= True;
			AssignFile(LogFile, 'errorlog.log');
			if not FileExists('errorlog.log') then
				Rewrite(LogFile)
			else
				Append(LogFile);

			Writeln(LogFile);
			MoriaLog(mlrInform, '--- Program starting');
			end;

//		Get SaveOnLevel setting
		i:= sl.IndexOfName('SaveOnLevel');
		if  (i = -1)
		or  (not TryStrToInt(sl.ValueFromIndex[i], v))
		or  (v = 0) then
			DebugSaveOnLevel:= False
		else
			begin
			DebugSaveOnLevel:= True;
			MoriaLog(mlrInform, '--- Enabling Debug SaveOnLevel');
			end;

//		Read screen settings
		iniFile.ReadSectionValues('SCREEN', sl);

//		Get screen mode (default to black and white)
		i:= sl.IndexOfName('Mode');
		if  (i = -1)
		or  (not TryStrToInt(sl.ValueFromIndex[i], v)) then
			screen.ScreenMode:= ssmBlackWhite
		else
			screen.ScreenMode:= TScreenMode(v);

//		Get cursor type
		i:= sl.IndexOfName('LargeCursor');
		if (i = -1)
		or (not TryStrToInt(sl.ValueFromIndex[i], v)) then
			screen.CursorSizeNormal
		else
			screen.CursorSizeBig;

		screen.ClrScr;

		iniFile.ReadSectionValues('MESSAGES', sl);
		i:= sl.IndexOfName('TermCompatibility');
		if  (i = -1)
		or  (not TryStrToInt(sl.ValueFromIndex[i], v)) then
			v:= 0;

		if  (CompareStr(GetEnvironmentVariable('TERM'), 'linux') = 0)
		and (screen.ScreenMode = ssmSixteenColour)
		and (v = 0) then
			begin
			MoriaLog(mlrWarn, 'CFG Detected potential screen mode issue.');

			screen.GotoXY(1, 1);
			screen.Writeln('A potential issue with the terminal you are using and the configured screen');
			screen.Writeln('mode has been detected.  The game may not be displayed correctly.');
			screen.Writeln;
			screen.Writeln('You should change the value for the screen mode to be eight colour or less in');
			screen.Writeln('the config.ini file if you experience any issues.');

			pause_exit(24, 0);

			screen.ClrScr;
			iniFile.WriteInteger('MESSAGES', 'TermCompatibility', 1);
			end;

		finally
		sl.Free;
		iniFile.Free;
		end;

	if ParamCount > 0 then
		if ParamStr(ParamCount) <> '/WIZARD' then
			finam:= ShortString(ParamStr(ParamCount));
	end;

procedure UserConfigure(const AIntro: Boolean = True);
	var
	dm: TScreenMode;
	dummy: AnsiChar;

	procedure DoDisplayIntro;
		begin
		clear(1, 1);
		screen.Writeln('Welcome to the Moria user configuration mode.');
		screen.Writeln;
		screen.Writeln('Here you will be able to change the screen mode and view the results of this');
		screen.Writeln('setting.');
		screen.Writeln;
		screen.Writeln('On the next screen, use <A..C> to set the screen mode:');
		screen.Writeln('    A :  Black and white');
		screen.Writeln('    B :  Eight (8) colour');
		screen.Writeln('    C :  Sixteen (16) colour');
		screen.Writeln;
		screen.Writeln('Use <F1..F3> to change the display (for testing):');
		screen.Writeln('    F1:  Display black and white grid');
		screen.Writeln('    F2:  Display eight (8) colour grid');
		screen.Writeln('    F3:  Display sixteen (16) colour grid');
		screen.Writeln;
		screen.Writeln('Use <Control>-Z or <Escape> to exit.');
		end;

	procedure DoDisplayGrid;
		var
		x,
		y,
		c: Integer;

		begin
		clear(1, 1);

		screen.BeginUpdate;
		try
			screen.GotoXY(20, 3);

			if dm = ssmSixteenColour then
				begin
				screen.Write(' 0123456789ABCDEF                     BF',
						StringOfChar(COLR_NORMAL, 40));
				c:= 16;
				end
			else if dm = ssmEightColour then
				begin
				screen.Write(' 01234567                             BF',
						StringOfChar(COLR_NORMAL, 40));
				c:= 8;
				end
			else
				begin
				screen.Write(' 01                                   BF',
						StringOfChar(COLR_NORMAL, 40));
				c:= 2;
				end;

			for y:= 0 to c - 1 do
				begin
				screen.GotoXY(20, 4 + y);
				screen.Write(AnsiString(Format('%x', [y])), COLR_NORMAL);

				for x:= 0 to c - 1 do
					screen.Write('#', AnsiChar((y shl 4) + x));

				screen.GotoXY(57, 4 + y);
				screen.Write(AnsiString(Format('%x', [y])), COLR_NORMAL);

				screen.Write('%%', AnsiChar(y shl 4) + AnsiChar(y));
				end;

			finally
			screen.EndUpdate;
			end;
		end;

	begin
	if AIntro then
		begin
		DoDisplayIntro;

		pause(24);
		end;

	dm:= screen.ScreenMode;
	while True do
		begin
		DoDisplayGrid;

		inkey(dummy);
		case dummy of
			#0:
				begin
				dummy:= AnsiChar(screen.ReadKey);

				if dummy = ';' then
					dm:= ssmBlackWhite
				else if dummy = '<' then
					dm:= ssmEightColour
				else if dummy = '=' then
					dm:= ssmSixteenColour;
				end;
			#3, #25, #26, #27:
				Break;
			'a', 'A':
				screen.ScreenMode:= ssmBlackWhite;
			'b', 'B':
				screen.ScreenMode:= ssmEightColour;
			'c', 'C':
				screen.ScreenMode:= ssmSixteenColour;
			end;
		end;
	end;

procedure Initialise;
	begin
{	Some neccesary initializations }
	msg_line    := 1;
	quart_height:= screen_height div 4;
	quart_width := screen_width div 4;
	dun_level   := 0;

	{ Grab a random seed from the clock }
	//seed:= get_seed;
	SetSeed(get_seed);

	read_data;

	{ Sort the objects by level }
	sort_objects;

	{ Init monster and treasure levels for allocate }
	init_m_level;
	init_t_level;

	{ Init the store inventories }
	store_init;

//todo Implement difficulty settings
	//if (COST_ADJ <> 1.00) then
	//	price_adjust;

	SetLength(newInventory[mitWornWielding], Succ(Ord(High(TMoriaInvenWear))));
	end;

procedure Finalise;
	var
	i: TMoriaInvenType;

	begin
	for i:= High(TMoriaInvenType) downto Low(TMoriaInvenType) do
		SetLength(newInventory[i], 0);
	end;

{ Attempt to open the intro file }
//[psect(setup$code)]
procedure Intro;//(var finam: vtype);
	var
//	xpos,
//	i1: Integer;
//	day_test,
	in_line: VType;
	file1: Text;
	file2: file of Key_Type;
	exit_flag: Boolean;
	cf: Boolean;
	i: Integer;
	dummy: AnsiChar;

	begin
	exit_flag:= False;

//dengland  There was a bug in DCC XE2 causing the string data in MORIA_MOR to
//		be lost.  I've corrected it now (by declaring the values in the variable
//		initialisation) but I don't know what caused it.
//	OutputDebugString(PWideChar('MORIA_MOR: ' + string(MORIA_MOR)));

	AssignFile(file1, string(MORIA_MOR));
	Reset(file1);
//	if (status(file1) = 0) then
	if  IOResult <> 0 then
		begin
		Rewrite(file1);
		if (IOResult = 0) then
			begin
//todo This should come before the file open and read above with a test for FileExists.

			writeln(file1, '                       *************************');
			in_line:= Format(
					AnsiString('                       **    Moria  UB %2d.%2d   **'),
					[CUR_VERSION_MAJ, CUR_VERSION_MIN]);

			writeln(file1, in_line);
			writeln(file1, '                       *************************');
			writeln(file1, '                   COPYRIGHT (c) Robert Alan Koeneke');
			writeln(file1, ' ');
			writeln(file1, 'Programers : Robert Alan Koeneke / University of Oklahoma');
			writeln(file1, '             Jimmey Wayne Todd   / University of Oklahoma');
			writeln(file1, ' ');
			writeln(file1, ' ');
			writeln(file1, 'Modified by: Bill Palter     / State University of New York');
			writeln(file1, '             Nick B Triantos / State University of New York');
			writeln(file1, '                                                           ');
			Writeln(file1, 'Ported to Delphi/Free Pascal by: ');
			Writeln(file1, '             Daniel England');
			writeln(file1, ' ');
			writeln(file1, 'Dungeon Master: This file may contain updates and news.');

			screen.Writeln('Created ', MORIA_MOR);
//			exit_flag:= true;
			end
		else
			begin
			screen.Writeln('Error in creating ', MORIA_MOR);
//			Halt(0);
			raise EMoriaTerminate.Create('Could not create intro file');
			end;
		end;
	CloseFile(file1);

	while True do
		begin
		i:= 1;
		cf:= False;
		clear(1, 1);
		AssignFile(file1, string(MORIA_MOR));
		Reset(file1);
		if  IOResult = 0 then
			begin
			screen.BeginUpdate;
			try
				repeat
					Readln(file1, in_line);
					if  (Length(in_line) > 0)
					and (in_line[1] = '#') then
						begin
						cf:= True;
						Continue;
						end;

					if not cf then
						screen.Writeln(in_line)
					else
						DecodeClrTextToScreen(in_line, i);

					until (eof(file1));

				finally
				screen.EndUpdate;
				end;
			end;
		CloseFile(file1);

		prt('[Press any key to continue, <F10> to confiure or <Control>-Z to exit]',
				STR_CLR_NMHHLT, 24, 5);

		inkey(dummy);
		erase_line(24, 1);

		case dummy of
			#0:
				begin
				dummy:= AnsiChar(screen.ReadKey);
				if dummy = 'D' then
					UserConfigure;
				end;
			#3, #25, #26 :
				begin
				sleep(10);
				raise EMoriaTerminate.Create('Pause exit');
				end;
			else
				Break;
			end;
		end;

	AssignFile(file2, string(MORIA_MAS));
	Reset(file2);
	if (IOResult <> 0) then
		begin
		Rewrite(file2);
		if (IOResult = 0) then
			begin
			screen.Writeln('Created ', moria_mas);
			CloseFile(file2);
			exit_flag:= true;
			end
		else
			begin
			screen.Writeln('Error in creating ', moria_mas);
//			Halt(0);
			raise EMoriaTerminate.Create('Could not create master file');
			end;
		end
	else
		CloseFile(file2);

{ Check for TOPTWENTY.DAT}

	AssignFile(file1, string(MORIA_TOP));
	Reset(file1);
//	if ((status(file1) <> 0) and (status(file1) <> 2)) then
//dengland I don't know what status 2 would have been.
	if (IOResult <> 0) then
		begin
		Rewrite(file1);
		if (IOResult = 0) then
			begin
			screen.Writeln('Created ', moria_top);
			CloseFile(file1);
			exit_flag:= true;
			end
		else
			begin
			screen.Writeln('Error in creating ', moria_top);
//			Halt(0);
			raise EMoriaTerminate.Create('Could not open top twenty file');
			end;
		end
	else
		CloseFile(file1);

	if (exit_flag) then
		begin
		screen.Writeln('Notice: System MORIA wizard should set the protection');
		screen.Writeln('        on  files  just created.  See INSTALL.HLP for');
		screen.Writeln('        help on setting protection on the files.');
		screen.Writeln('Notice: File MORIA.DAT may be edited to contain  news');
		screen.Writeln('        items, etc...');
//		Halt(0);
		raise EMoriaTerminate.Create('General file error');
		end;

	if Length(finam) = 0 then
		DisplayTopTwenty;
	end;

procedure Start;
	begin
	{ Generate a character, or retrieve old one... }
	if (Length(finam) > 0) then
		begin { Retrieve character }
		generate:= get_char(finam);
		change_name;
		magic_init(randes_seed)
		end
	else
		begin { Create character }
		create_character;
		char_inven_init;
		if (class_[py.misc.pclass].mspell) then
			begin { Magic realm }
			learn_spell(msg_flag);
			gain_mana(int_adj)
			end
		else if (class_[py.misc.pclass].pspell) then
			begin { Clerical realm }
			learn_prayer;
			gain_mana(wis_adj)
			end;
		if (class_[py.misc.pclass].espell) then
			begin
			learn_extra(msg_flag);
			gain_mana(wis_adj)
			end;

		py.misc.cmana:= py.misc.mana;
		randes_seed  := GetSeed; { Description seed }
		town_seed    := GetSeed; { Town generation seed }
		magic_init(randes_seed);
		generate:= True
		end;

	with py.misc do { This determines the maximum player level }
		player_max_exp:= Trunc(player_exp[max_player_level - 1] * expfact);
	end;

procedure Play;
	var
	flag: Boolean;

	begin
	{ begin the game }
	clear(1, 1);
	prt_stat_block;

	{ Loop till dead, or exit }
	repeat
		if  generate then
			begin
			generate_cave; { New level }

			if DebugSaveOnLevel then
				begin
				DoSaveChar('_DEBUG.SAV', flag, False);
				inSave:= False;

				if not flag then
					MoriaLog(mlrWarn, '--- Debug save failed.');
				end;
			end;

		dungeon;           { Dungeon logic }

		generate:= True;
		until (death);

	upon_death; { Character gets buried }
	end;

procedure CrashDump;
	var
	flag: Boolean;

	begin
	if not inSave then
		try
			Writeln('*** Writing player crash dump...');
			DoSaveChar('CRASH.DUMP', flag, False);
			inSave:= False;

			if flag then
				Writeln('*** Writing crash dump SUCCEEDED')
			else
				Writeln('*** Writing crash dump NOT DONE!');

			except
			Writeln('*** Writing crash dump *FAILED*!');
			end
	else
		Writeln('*** Could NOT write crash dump');
	end;



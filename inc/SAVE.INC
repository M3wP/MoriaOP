{ Data Corruption means character is dead, or save file was screwed with.
  Keep them guessing as to what is actually wrong.}
//[psect(save$code)]
procedure data_exception;
	begin
	clear(1,1);
	prt('%MORIA-F-ATTREUSE,Attempted reuse of a character',1,1);
	prt('',2,1);
	screen.Writeln;
//	Halt(0);
	raise EMoriaTerminate.Create('Attempted reuse of character');
	end;

//[psect(save$code)]
procedure data_exception1;
	begin
	clear(1,1);
	prt('%MORIA-F-BADCHECK,Data Exception Error,Corruption',1,1);
	prt('',2,1);
	screen.Writeln;
//	Halt(0);
	raise EMoriaTerminate.Create('Data Exeption Error,Corruption');
	end;


{ Uses XOR function to encode data }
//[psect(save$code)]
procedure coder(var line : ntype);
	var
	i1: integer;
	i2,i3,i4: unsigned;

	begin
	for i1 := 1 to length(line) do
		begin
		i2 := {uint}(ord(line[i1]));
		i3 := {uint}(randint(256)-1);
		i4 := (i2 xor i3);
		line[i1] := AnsiChar(i4);
		end;
	end;

//dengland  Encryption of save files has been disabled since there is a conflict
//		with Readln.  I will need to do the decyption and reading in a different
//		way for it to work.  I'm honestly amazed it ever worked on VMS
//todo Implement a better decryption and reading algorythm.

{ Encrypts a line of text, complete with a data-check sum }
//[psect(save$code)]
procedure encrypt(var line : ntype);
(*	var
	i1,i2: integer;
	temp: ntype;*)

	begin
(*	i2 := 0;
	for i1 := 1 to length(line) do
		i2 := i2 + ord(line[i1]) + i1;
	temp := line;
//	writev(line,i2:1,' ',temp);
	line:= AnsiString(IntToStr(i2) + ' ' + string(temp));
	coder(line);*)
	end;


{ Decrypts a line of text, complete with a data-check sum }
//[psect(save$code)]
procedure decrypt(var line : ntype);
(*	var
	i1,i2,i3        : integer;
	temp            : ntype;
//	tmp             : AnsiChar;
	p: Integer;
	s: AnsiString;*)

	begin
(*	i2 := 0;
	coder(line);
	temp := line;

//	readv(temp,i3,tmp,line);
//	tmp:= #0;
	line:= '';
	s:= Trim(temp);
	p:= index(s, ' ');
	if p > 0 then
		begin
		i3:= StrToInt(string(Copy(s, 1, p - 1)));
		s:= Copy(s, p + 1, MaxInt);
//		if Length(s) > 0 then
//			tmp:= s[1];
		if Length(s) > 1 then
			line:= Copy(s, 2, MaxInt);
		end
	else
		i3:= StrToInt(string(s));

	for i1 := 1 to length(line) do
		i2 := i2 + ord(line[i1]) + i1;
	if (i2 <> i3) then
		data_exception1;*)
	end;

procedure DoSaveChar(fnam: ShortString; var flag: Boolean; ADoDisplay: Boolean);
	var
	tot_monsters,
	tot_treasure: integer;
	i1,
	i2,
	trys: integer;
	xfloor: unsigned;
	save_seed: LongInt;
	saveline: vtype;
	temp: string[70];
	out_rec,
	title1,
	title2: ntype;
	f1: text;
	f2: file of key_type;
	file_flag: boolean;
	t: TMoriaInvenType;

	begin
	inSave:= True;
	flag:= False;

	AssignFile(f1, string(fnam));
	Rewrite(f1);

	if (IOResult <> 0) then
		begin
		if ADoDisplay then
			msg_print('Error creating> ' + fnam);
		end
	else
		flag := True;

{ Make an attempt to open the MASTER file               -JWT-   }
	if (flag) then
		begin
//dengland	I think this is what is required
		AssignFile(f2, string(moria_mas));
		if FileExists(string(moria_mas)) then
			begin
			Reset(f2);
			Seek(f2, FileSize(f2));
			end
		else
			Rewrite(f2);

		if (IOResult <> 0) then
			begin
			if ADoDisplay then
				msg_print('Error saving character, contact Moria wizard.');
			CloseFile(f1);
			flag := false;
			end;
		end;

{ Make an attempt to create a unique ID and write to MASTER -RAK-}
	if (flag) then
		begin
		trys := 0;
		file_flag := false;
		repeat
			temp:= '';
			for i1 := 1 to 70 do
//				temp[i1] := AnsiChar(31+randint(95));
				temp:= temp + AnsiChar(31+randint(95));
			title1 := temp;
//			seed := encrypt_seed1;
			SetSeed(encrypt_seed1);
			encrypt(title1);
			for i1 := 1 to 70 do
				key_rec.file_id[i1] := title1[i1];

//			f2^ := key_rec;
//			put(f2,error:=continue);
			Write(f2, key_rec);

			if IOResult <> 0 then
				begin
				trys := trys + 1;
				if (trys > 5) then
					begin
					file_flag := true;
					flag := false;
					msg_print('Error in writing to MASTER.');
					end;
				end
			else
				file_flag := true;
			until(file_flag);
		CloseFile(f2);
		end;

{ If ID was written to MASTER, continue saving          -RAK-   }
	if (flag) then
		begin
		save_seed := get_seed;
		title2:= PadLeft(AnsiString(IntToStr(save_seed)), 12) + ' ' + temp;
//		seed := encrypt_seed2;
		SetSeed(encrypt_seed2);
		encrypt(title2);
		Writeln(f1, title2);
//		seed := save_seed;
		SetSeed(save_seed);
{ Message to player on what is happening}
		if ADoDisplay then
			begin
			clear(1,1);
{       	prt('Saving character...',1,1);             }
			saveline:= 'Saving character as ' + fnam + '...';
{       	prt('Saving character as ',fnam,'...',1,1);}
			prt(saveline,1,1);
			put_qio;
			end;
{ Version number of Moria               }
		out_rec:= Format(
				AnsiString('%2d %2d'), [CUR_VERSION_MAJ, CUR_VERSION_MIN]);
		encrypt(out_rec);
		Writeln(f1, out_rec);

		{ Write out the player record.  }
		with py.misc do
			begin
			out_rec:= name;
			encrypt(out_rec);
			Writeln(f1, out_rec);

			out_rec:= race;
			encrypt(out_rec);
			Writeln(f1, out_rec);

			out_rec:= sex;
			encrypt(out_rec);
			Writeln(f1, out_rec);

			out_rec:= tclass;
			encrypt(out_rec);
			Writeln(f1, out_rec);

			out_rec:= title;
			encrypt(out_rec);
			Writeln(f1, out_rec);

			for i1 := 1 to 5 do
				begin
				out_rec:= history[i1];
				encrypt(out_rec);
				writeln(f1, out_rec {,error:=continue});
				end;

			out_rec:= Format(
					AnsiString('%d %d %d %d %d %d %d %d %d %d %d %d %2.1f'),
					[char_row, char_col, pclass, prace, age, ht, wt, sc,
					max_exp, exp, lev, max_lev, expfact]);

			encrypt(out_rec);
			writeln(f1, out_rec {,error:=continue});

			out_rec:= Format(
					AnsiString('%d %d %d %d %d %d %f %d %f %d %d %d %d %d %d %d %d %d %d %d %d'),
					[srh, fos, stl, bth, bthb, mana, cmana, mhp, chp, au,
					ptohit, ptodam, pac, ptoac, dis_th, dis_td, dis_ac,
					dis_tac, disarm, save, hitdie]);

			encrypt(out_rec);
			writeln(f1, out_rec {,error:=continue});

			out_rec:= Format(
					AnsiString('%d %d %d %d %d %d %d %d'),
//fixme Need to remove this value, equip_ctr
//					[inven_ctr, inven_weight, equip_ctr, dun_level,
					[inven_ctr, inven_weight, inven_ctr, dun_level,
					missle_ctr, mon_tot_mult, ($F and turn), randes_seed]);

			encrypt(out_rec);
			writeln(f1, out_rec {,error:=continue});
			end;

{ Write out the inventory records.      }
		for t:= Low(TMoriaInvenType) to Pred(High(TMoriaInvenType)) do
			for i1 := 0 to Length(newInventory[t]) - 1 do
				begin
				out_rec:= newInventory[t, i1].tchar + newInventory[t, i1].name;
				encrypt(out_rec);
				writeln(f1, out_rec {,error:=continue});

				out_rec:= newInventory[t, i1].damage;
				encrypt(out_rec);
				writeln(f1, out_rec {,error:=continue});

				with newInventory[t, i1] do
					out_rec:= Format(
							AnsiString('%d %d %d %d %d %d %d %d %d %d %d %d'),
							[tval, subval, weight, number, tohit, todam, ac,
							toac, p1, flags, level, cost]);

				encrypt(out_rec);
				writeln(f1, out_rec {,error:=continue});
				end;

{ Write out the equipment records.      }
		for i1:= Ord(Low(TMoriaInvenWear)) to Ord(High(TMoriaInvenWear)) do
			begin
			out_rec:= newInventory[mitWornWielding, i1].tchar +
					newInventory[mitWornWielding, i1].name;
			encrypt(out_rec);
			writeln(f1, out_rec {,error:=continue});

			out_rec:= newInventory[mitWornWielding, i1].damage;
			encrypt(out_rec);
			writeln(f1, out_rec {,error:=continue});

			with newInventory[mitWornWielding, i1] do
				out_rec:= Format(
						AnsiString('%d %d %d %d %d %d %d %d %d %d %d %d'), [tval,
						subval, weight, number, tohit, todam, ac, toac, p1,
						flags, level, cost]);

			encrypt(out_rec);
			writeln(f1, out_rec {,error:=continue});
			end;

		with py.stat do
			begin
			out_rec:= Format(
					AnsiString('%d %d %d %d %d %d %d %d %d %d %d %d %d'), [str, cstr,
					dex, cdex, con, ccon, int, cint, wis, cwis, chr, cchr, avail]);
			encrypt(out_rec);
			writeln(f1, out_rec {,error:=continue});
			end;

		with py.flags do
			begin
			out_rec:= Format(
					AnsiString('%d %d %d %d %d %d %d %d %d %d'), [status, blind,
					confused, food, food_digested, protection, speed,
					afraid, poisoned, Ord(see_inv)]);

			encrypt(out_rec);
			writeln(f1, out_rec {,error:=continue});

			out_rec:= Format(
					AnsiString('%d %d %d %d %d %d'), [fast, slow, protevil,
					Ord(teleport), Ord(free_act), Ord(slow_digest)]);
			encrypt(out_rec);
			writeln(f1, out_rec {,error:=continue});

			out_rec:= Format(
					AnsiString('%d %d %d %d %d %d %d'), [Ord(aggravate),
					Ord(sustain_str), Ord(sustain_int), Ord(sustain_wis),
					Ord(sustain_con), Ord(sustain_dex), Ord(sustain_chr)]);
			encrypt(out_rec);
			writeln(f1, out_rec {,error:=continue});

			out_rec:= Format(
					AnsiString('%d %d %d %d %d %d %d'), [Ord(fire_resist),
					Ord(cold_resist), Ord(acid_resist), Ord(regenerate),
					Ord(lght_resist), Ord(ffall), Ord(confuse_monster)]);
			encrypt(out_rec);
			writeln(f1, out_rec {,error:=continue});

			out_rec:= Format(
					AnsiString('%d %d %d %d %d %d %d %d %d %d %d'), [image, invuln,
					hero, shero, blessed, resist_heat, resist_cold,
					detect_inv, word_recall, see_infra, tim_infra]);

			encrypt(out_rec);
			writeln(f1, out_rec {,error:=continue});
			end;

		for i1 := 1 to 31 do
			with mage_spell[py.misc.pclass,i1] do
				begin
				out_rec:= Format(
						AnsiString('%5d %5d'), [Ord(learned), sexp]);

				encrypt(out_rec);
				writeln(f1, out_rec {,error:=continue})
				end;

		for i1 := 1 to 31 do
			with priest_spell[py.misc.pclass,i1] do
				begin
				out_rec:= Format(
						AnsiString('%5d %5d'), [Ord(learned), sexp]);
				encrypt(out_rec);
				writeln(f1, out_rec {,error:=continue})
				end;

		for i1 := 1 to 31 do
			with extra_spell[py.misc.pclass,i1] do
				begin
				out_rec:= Format(
						AnsiString('%5d %5d'), [Ord(learned), sexp]);
				encrypt(out_rec);
				writeln(f1, out_rec {,error:=continue})
				end;

{ Write the important dungeon info and floor    -RAK-   }
		out_rec:= Format(
				AnsiString('%d %d %d %d'), [cur_height, cur_width, max_panel_rows,
				max_panel_cols]);
		encrypt(out_rec);
		writeln(f1, out_rec {,error:=continue});

		{ Save the floor        }
		tot_treasure := 0;
		for i1 := 1 to cur_height do
			begin
			out_rec := PadRight(' ', cur_width);
			for i2 := 1 to cur_width do
				begin
				with cave[i1,i2] do
					begin
//dengland	Always set the high bit on floor so its distinguised from line end
//		markers ($0A is getting written as floor tile)
					xfloor := fval or $80;
					if (fopen) then
						xfloor := (xfloor or $10);
					if (pl) then
						xfloor := (xfloor or $20);
					if (fm) then
						xfloor := (xfloor or $40);
					out_rec[i2] := AnsiChar(xfloor);
					if (tptr > 0) then
						tot_treasure := tot_treasure + 1;
					end;
				end;
			encrypt(out_rec);
			writeln(f1, out_rec {,error:=continue});
			end;

		{ Save the Treasure List                }
//		writev(out_rec, tot_treasure:1);
		out_rec:= Format(
				AnsiString('%d'), [tot_treasure]);
		encrypt(out_rec);
		writeln(f1, out_rec {,error:=continue});
		for i1 := 1 to cur_height do
			for i2 := 1 to cur_width do
				if (cave[i1,i2].tptr > 0) then
					with t_list[cave[i1,i2].tptr] do
						begin
						out_rec:= Format(
								AnsiString('%d %d'), [i1, i2]);
						encrypt(out_rec);
						writeln(f1, out_rec {,error:=continue});

						out_rec:= tchar + name;
						encrypt(out_rec);
						writeln(f1, out_rec {,error:=continue});

						out_rec:= damage;
						encrypt(out_rec);
						writeln(f1, out_rec {,error:=continue});

						out_rec:= Format(
								AnsiString('%d %d %d %d %d %d %d %d %d %d %d %d'),
								[tval, subval, weight, number, tohit,
								todam, ac, toac, p1, flags, level, cost]);
						encrypt(out_rec);
						writeln(f1, out_rec {,error:=continue});
						end;

		{ Save identified list                  }
		out_rec := '';
		for i1 := 1 to max_objects do
			begin
			if (object_ident[i1]) then
				out_rec := out_rec + 'T'
			else
				out_rec := out_rec + 'F';
			end;
		encrypt(out_rec);
		writeln(f1, out_rec {,error:=continue});

		{ Save the Monster List                 }
		i1 := muptr;
		tot_monsters := 0;
		if (i1 > 0) then
			repeat
				tot_monsters := tot_monsters + 1;
				with m_list[i1] do
					i1 := nptr;
				until (i1 = 0);
		out_rec:= Format(
				AnsiString('%d'), [tot_monsters]);
		encrypt(out_rec);
		writeln(f1, out_rec {,error:=continue});
		i1 := muptr;
		if (i1 > 0) then
			repeat
				with m_list[i1] do
					begin
					out_rec:= Format(
							AnsiString('%d %d %d %d %d %d %d %d %d %d %d 0'), [fy, fx, mptr, hp,
							cspeed, csleep, cdis, Ord(ml), Ord(confused), Ord(elite),
							stoleau]);
					encrypt(out_rec);
					writeln(f1, out_rec {,error:=continue});
					i1 := nptr;
					end;
				until (i1 = 0);

		{ Save the town level stores            }
		out_rec:= Format(AnsiString('%12d'), [town_seed]);
		encrypt(out_rec);
		writeln(f1, out_rec {,error:=continue});
		for i1 := 1 to max_stores do
			with store[i1] do
				begin
				{ Save items...                 }
				out_rec:= Format(AnsiString('%d'), [store_ctr]);
				encrypt(out_rec);
				writeln(f1, out_rec {,error:=continue});
				for i2 := 1 to store_ctr do
					with store_inven[i2].sitem do
						begin
						out_rec:= Format(
								AnsiString('%d'), [store_inven[i2].scost]);
						encrypt(out_rec);
						writeln(f1, out_rec {,error:=continue});
						out_rec:= tchar + name;
						encrypt(out_rec);
						writeln(f1, out_rec {,error:=continue});
						out_rec:= damage;
						encrypt(out_rec);
						writeln(f1, out_rec {,error:=continue});
						out_rec:= Format(
								AnsiString('%d %d %d %d %d %d %d %d %d %d %d %d'),
								[tval, subval, weight, number, tohit,
								todam, ac, toac, p1, flags, level, cost]);

						encrypt(out_rec);
						writeln(f1, out_rec {,error:=continue});
						end;
				if (store_open > turn) then
					store_open := store_open - turn + 15
				else
					store_open := 0;
				out_rec:= Format(
						AnsiString('%d %d %d'), [owner, insult_cur, store_open]);
				encrypt(out_rec);
				writeln(f1, out_rec {,error:=continue});
				end;
		CloseFile(f1);
		end;
	end;

{ Actual save procedure }
//[psect(save$code)]
function save_char: Boolean;
	var
	fnam: string[70];
	flag: Boolean;
	out_rec: NType;

	begin
	prt('Enter filename:',1,1);
	flag := false;
{ Open the user's save file }
	if (get_string(fnam,1,17,60)) then
		begin
		if (length(fnam) = 0) then
			fnam := 'MORIACHR.SAV';

		DoSaveChar(fnam, flag, True);
		end;

	if (flag) then
		begin
//		writev(out_rec,'Character Saved. [Moria Version ', cur_version:5:2,']');
		out_rec:= Format(
				AnsiString('Character saved. [Moria version %d.%2.2d]'),
				[CUR_VERSION_MAJ, CUR_VERSION_MIN]);

		prt(out_rec,1,1);
		screen.WriteLn;

//		Halt(0);
		raise EMoriaTerminate.Create('User saved');
		end;
	save_char:= flag;

//	seed:= get_seed;
	SetSeed(get_seed);
	end;


{ Restore a saved game                          -RAK- & -JWT-   }
//[psect(save$code)]
function get_char(fnam: vtype): Boolean;
	var
	tot_treasures,
	tot_monsters,
	mwk: Integer;
	i1,
	i2,
	i3,
	i4: Integer;
//	dummy: Integer;
	xfloor,
	save_seed: LongInt;
	save_ver_maj,
	save_ver_min: Byte;
	in_rec,
	temp: ntype;
	temp_id: string[70];
	f1: text;
	f2: file of key_type;

	dun_flag: Boolean;
	old_flag: Boolean;

	fb1, fb2: Extended;
	ib1, ib2, ib3, ib4, ib5, ib6, ib7, ib8, ib9, ib10,
	ib11, ib12, ib13, ib14, ib15, ib16, ib17, ib18, ib19: Integer;

	t: TMoriaInvenType;

	procedure do_finddel_key;
(*		var
		i, j: Integer;
		key_rec: Key_Type;
		sl: TStringList;*)

		begin
(*		sl:= TStringList.Create;
		try
			Seek(f2, 0);
			while not Eof(f2) do
				begin
				Read(f2, key_rec);
				sl.AddObject(string(key_rec.file_id), TObject(key_rec.seed));
				end;
			Seek(f2, 0);

			i:= sl.IndexOf(string(temp_id));

			if i > -1 then
				begin
				sl.Delete(i);

				for i:= 0 to sl.Count - 1 do
					begin
//					key_rec.file_id:= AnsiString(sl[i]);
					FillChar(key_rec.file_id[1], 70, #0);
					for j:= 1 to Length(sl[i]) do
						key_rec.file_id[j]:= AnsiChar(sl[i][j]);
					key_rec.seed:= Integer(sl.objects[i]);
					Write(f2, key_rec);
					end;
				end
			else
				data_exception;

			finally
			sl.Free;
			end;*)
		end;

	begin
	MoriaLog(mlrInform, '--- Loading character');

	dun_flag:= False;
	old_flag:= False;

	clear(1, 1);
	AssignFile(f1, string(fnam));
	Reset(f1);
	if (IOResult <> 0) then
		begin
		prt('Error opening> ' + fnam, 1, 1);
		prt('', 2, 1);
//		exit;
		screen.Writeln;
//		Halt(0);
		raise EMoriaTerminate.Create('Error opening save file');
		end;
	readln(f1, in_rec);
//	seed:= encrypt_seed2;
	SetSeed(encrypt_seed2);
	decrypt(in_rec);
	temp:= Copy(in_rec, 1, 12);
	save_seed:= StrToInt(string(temp));
	temp:= Copy(in_rec, 14, 70);
//	seed:= encrypt_seed1;
	SetSeed(encrypt_seed1);
	encrypt(temp);
	temp_id:= '';
	for mwk:= 1 to 70 do { -Opusii+MWK 6/27/87 }
		temp_id:= temp_id + temp[mwk];
	AssignFile(f2, string(MORIA_MAS));
	Reset(f2);
	if (IOResult <> 0) then
		begin
		prt('ERROR opening file MASTER.', 1, 1);
//		exit;
		screen.Writeln;
//		Halt(0);
		raise EMoriaTerminate.Create('Error opening master file');
		end;

//	findk(f2, 0, temp_id, eql, ERROR:= continue);
//	delete(f2, ERROR:= continue);
//	if (status(f2) <> 0) then
//		data_exception;
	do_finddel_key;

	CloseFile(f2);
//	seed:= save_seed;
	SetSeed(save_seed);
	prt('Restoring character...', 1, 1);
	put_qio;
	readln(f1, in_rec);
	decrypt(in_rec);
	sscanf(in_rec, '%d %d', [@ib1, @ib2]);
	save_ver_maj:= ib1;
	save_ver_min:= ib2;

	if (save_ver_maj <> CUR_VERSION_MAJ)
	or (save_ver_min <> CUR_VERSION_MIN) then
		begin
		prt('Save file is a previous version.', 2, 1);

//		writev(in_rec, '  [Save file version ', save_version: 5: 2, ']');
		in_rec:= Format(
				AnsiString('  [Save file version %d.%2.2d]'), [save_ver_min, save_ver_maj]);

		prt(in_rec, 3, 1);

//		writev(in_rec, '  [Moria version     ', cur_version: 5: 2, ']');
		in_rec:= Format(
				AnsiString('  [Moria version     %d.%2.2d]'), [CUR_VERSION_MAJ,
				CUR_VERSION_MIN]);
		prt(in_rec, 4, 1);

		if (save_ver_maj > 4) then
			begin
			prt('Updating character for newer version...', 5, 1);

			dun_flag:= save_ver_min < 1;
			old_flag:= True;
			end
		else
			begin
//			exit;
			prt('Save file incompatible.', 5, 1);
			screen.Writeln;

//			Halt(0);
			raise EMoriaTerminate.Create('Save file incompatible');
			end;

		pause(24);
		end;

	readln(f1, in_rec);
	decrypt(in_rec);
	with py.misc do
//		readv(in_rec, name);
		name:= in_rec;

	readln(f1, in_rec);
	decrypt(in_rec);
	with py.misc do
//		readv(in_rec, race);
		race:= in_rec;

	readln(f1, in_rec);
	decrypt(in_rec);
	with py.misc do
//		readv(in_rec, sex);
		sex:= in_rec;

	readln(f1, in_rec);
	decrypt(in_rec);
	with py.misc do
//		readv(in_rec, tclass);
		tclass:= in_rec;

	readln(f1, in_rec);
	decrypt(in_rec);
	with py.misc do
//		readv(in_rec, title);
		title:= in_rec;

	for i1:= 1 to 5 do
		begin
		readln(f1, in_rec);
		decrypt(in_rec);
		py.misc.history[i1]:= in_rec;
		end;

	readln(f1, in_rec);
	decrypt(in_rec);
	with py.misc do
		begin
//		readv(in_rec, char_row, char_col, pclass, prace, age, ht, wt, sc,
//			max_exp, exp, lev, max_lev, expfact);
		sscanf(in_rec, '%d %d %d %d %d %d %d %d %d %d %d %d %f', [@ib1, @ib2,
				@ib3, @ib4, @ib5, @ib6, @ib7, @ib8, @ib9, @ib10, @ib11, @ib12,
				@fb1]);
		char_row:= ib1;
		char_col:= ib2;
		pclass:= ib3;
		prace:= ib4;
		age:= ib5;
		ht:= ib6;
		wt:= ib7;
		sc:= ib8;
		max_exp:= ib9;
		exp:= ib10;
		lev:= ib11;
		max_lev:= ib12;
		expfact:= fb1;
		end;

	readln(f1, in_rec);
	decrypt(in_rec);
	with py.misc do
		begin
//		readv(in_rec, srh, fos, stl, bth, bthb, mana, cmana, mhp, chp, au,
//			ptohit, ptodam, pac, ptoac, dis_th, dis_td, dis_ac, dis_tac, disarm,
//			save, hitdie);
		sscanf(in_rec, '%d %d %d %d %d %d %f %d %f %d %d %d %d %d %d %d %d %d %d %d %d',
					[@ib1, @ib2, @ib3, @ib4, @ib5, @ib6, @fb1, @ib7, @fb2, @ib8,
					@ib9, @ib10, @ib10, @ib11, @ib13, @ib14, @ib15, @ib16,
					@ib17, @ib18, @ib19]);
		srh:= ib1;
		fos:= ib2;
		stl:= ib3;
		bth:= ib4;
		bthb:= ib5;
		mana:= ib6;
		cmana:= fb1;
		mhp:= ib7;
		chp:= fb2;
		au:= ib8;
		ptohit:= ib9;
		ptodam:= ib10;
		pac:= ib11;
		ptoac:= ib12;
		dis_th:= ib13;
		dis_td:= ib14;
		dis_ac:= ib15;
		dis_tac:= ib16;
		disarm:= ib17;
		save:= ib18;
		hitdie:= ib19;
		end;

	readln(f1, in_rec);
	decrypt(in_rec);
//	readv(in_rec, inven_ctr, inven_weight, equip_ctr, dun_level, missle_ctr,
//		mon_tot_mult, turn, randes_seed);
	sscanf(in_rec, '%d %d %d %d %d %d %d %d', [@ib1, @ib2, @ib3, @ib4, @ib5,
			@ib6, @ib7, @ib8]);

//dengland Reaccumulate the value in InvenAdd
//	inven_ctr:= ib1;
	inven_ctr:= 0;

	inven_weight:= ib2;
//fixme Remove this from file
//	equip_ctr:= ib3;
	dun_level:= ib4;
	missle_ctr:= ib5;
	mon_tot_mult:= ib6;
	turn:= ib7;
	randes_seed:= ib8;

	MoriaLog(mlrInform, 'Dungeon level: ' + AnsiString(IntToStr(dun_level)));

	{ Read in the inventory records. }
	for i1:= 1 to ib1 do
		begin
		readln(f1, in_rec);
		decrypt(in_rec);
//		readv(in_rec, inventory[i1].tchar, inventory[i1].name);
		invenTemp.tchar:= in_rec[1];
		invenTemp.name:= Copy(in_rec, 2, SizeOf(TType));

		readln(f1, in_rec);
		decrypt(in_rec);
//		readv(in_rec, inventory[i1].damage);
		invenTemp.damage:= in_rec;

		readln(f1, in_rec);
		decrypt(in_rec);
		with invenTemp do
			begin
//			readv(in_rec, tval, subval, weight, number, tohit, todam, ac, toac,
//				p1, flags, level, cost);
			sscanf(in_rec, '%d %d %d %d %d %d %d %d %d %d %d %d', [@ib1, @ib2,
					@ib3, @ib4, @ib5, @ib6, @ib7, @ib8, @ib9, @ib10, @ib11, @ib12]);
			tval:= ib1;
			subval:= ib2;
			weight:= ib3;
			number:= ib4;
			tohit:= ib5;
			todam:= ib6;
			ac:= ib7;
			toac:= ib8;
			p1:= ib9;
			flags:= ib10;
			level:= ib11;
			cost:= ib12;
			end;

		InvenAdd(@invenTemp, t, ib1);
		end;

	{ Read in the equipment records. }
	for i1:= Ord(Low(TMoriaInvenWear)) to Ord(High(TMoriaInvenWear)) do
		begin
		readln(f1, in_rec);
		decrypt(in_rec);
//		readv(in_rec, inventory[i1].tchar, inventory[i1].name);
		newInventory[mitWornWielding, i1].tchar:= in_rec[1];
		newInventory[mitWornWielding, i1].name:= Copy(in_rec, 2, SizeOf(TType));

		readln(f1, in_rec);
		decrypt(in_rec);
//		readv(in_rec, inventory[i1].damage);
		newInventory[mitWornWielding, i1].damage:= in_rec;

		readln(f1, in_rec);
		decrypt(in_rec);
		with newInventory[mitWornWielding, i1] do
			begin
//			readv(in_rec, tval, subval, weight, number, tohit, todam, ac, toac,
//				p1, flags, level, cost);
			sscanf(in_rec, '%d %d %d %d %d %d %d %d %d %d %d %d', [@ib1, @ib2,
					@ib3, @ib4, @ib5, @ib6, @ib7, @ib8, @ib9, @ib10, @ib11, @ib12]);

			tval:= ib1;
			subval:= ib2;
			weight:= ib3;
			number:= ib4;
			tohit:= ib5;
			todam:= ib6;
			ac:= ib7;
			toac:= ib8;
			p1:= ib9;
			flags:= ib10;
			level:= ib11;
			cost:= ib12;
			end;
		end;

	CalculateStatAdjustments;

	readln(f1, in_rec);
	decrypt(in_rec);
	with py.stat do
		begin
//		readv(in_rec, str, cstr, dex, cdex, con, ccon, int, cint, wis, cwis,
//			chr, cchr);
		if old_flag then
			sscanf(in_rec, '%d %d %d %d %d %d %d %d %d %d %d %d', [@ib1, @ib2,
					@ib3, @ib4, @ib5, @ib6, @ib7, @ib8, @ib9, @ib10, @ib11, @ib12])
		else
			sscanf(in_rec, '%d %d %d %d %d %d %d %d %d %d %d %d %d', [@ib1, @ib2,
					@ib3, @ib4, @ib5, @ib6, @ib7, @ib8, @ib9, @ib10, @ib11,
					@ib12, @ib13]);

		str:= ib1;
		cstr:= ib2;
		dex:= ib3;
		cdex:= ib4;
		con:= ib5;
		ccon:= ib6;
		int:= ib7;
		cint:= ib8;
		wis:= ib9;
		cwis:= ib10;
		chr:= ib11;
		cchr:= ib12;

		if old_flag then
			avail:= 0
		else
			avail:= ib13;
		end;

	with py.flags do
		begin
		readln(f1, in_rec);
		decrypt(in_rec);
//		readv(in_rec, status, blind, confused, food, food_digested, protection,
//			speed, afraid, poisoned, see_inv);
		sscanf(in_rec, '%d %d %d %d %d %d %d %d %d %d', [@ib1, @ib2,
				@ib3, @ib4, @ib5, @ib6, @ib7, @ib8, @ib9, @ib10]);
		status:= ib1;
		blind:= ib2;
		confused:= ib3;
		food:= ib4;
		food_digested:= ib5;
		protection:= ib6;
		speed:= ib7;
		afraid:= ib8;
		poisoned:= ib9;
		see_inv:= Boolean(ib10);

		readln(f1, in_rec);
		decrypt(in_rec);
//		readv(in_rec, fast, slow, protevil, teleport, free_act, slow_digest);
		sscanf(in_rec, '%d %d %d %d %d %d', [@ib1, @ib2, @ib3, @ib4, @ib5, @ib6]);
		fast:= ib1;
		slow:= ib2;
		protevil:= ib3;
		teleport:= Boolean(ib4);
		free_act:= Boolean(ib5);
		slow_digest:= Boolean(ib6);

		readln(f1, in_rec);
		decrypt(in_rec);
//		readv(in_rec, aggravate, sustain_str, sustain_int, sustain_wis,
//			sustain_con, sustain_dex, sustain_chr);
		sscanf(in_rec, '%d %d %d %d %d %d %d', [@ib1, @ib2, @ib3, @ib4, @ib5,
				@ib6, @ib7]);
		aggravate:= Boolean(ib1);
		sustain_str:= Boolean(ib2);
		sustain_int:= Boolean(ib3);
		sustain_wis:= Boolean(ib4);
		sustain_con:= Boolean(ib5);
		sustain_dex:= Boolean(ib6);
		sustain_chr:= Boolean(ib7);

		readln(f1, in_rec);
		decrypt(in_rec);
//		readv(in_rec, fire_resist, cold_resist, acid_resist, regenerate,
//			lght_resist, ffall, confuse_monster);
		sscanf(in_rec, '%d %d %d %d %d %d %d', [@ib1, @ib2, @ib3, @ib4, @ib5,
				@ib6, @ib7]);
		fire_resist:= Boolean(ib1);
		cold_resist:= Boolean(ib2);
		acid_resist:= Boolean(ib3);
		regenerate:= Boolean(ib4);
		lght_resist:= Boolean(ib5);
		ffall:= Boolean(ib6);
		confuse_monster:= Boolean(ib7);

		readln(f1, in_rec);
		decrypt(in_rec);
//		readv(in_rec, image, invuln, hero, shero, blessed, resist_heat,
//			resist_cold, detect_inv, word_recall, see_infra, tim_infra);
		sscanf(in_rec, '%d %d %d %d %d %d %d %d %d %d %d', [@ib1, @ib2,
				@ib3, @ib4, @ib5, @ib6, @ib7, @ib8, @ib9, @ib10, @ib11]);

		image:= ib1;
		invuln:= ib2;
		hero:= ib3;
		shero:= ib4;
		blessed:= ib5;
		resist_heat:= ib6;
		resist_cold:= ib7;
		detect_inv:= ib8;
		word_recall:= ib9;
		see_infra:= ib10;
		tim_infra:= ib11;
		end;

	{ Older version characters do not store experience... }
	if (dun_flag) then
		begin
		for i1:= 1 to 31 do
			begin
			readln(f1, in_rec);
			decrypt(in_rec);
//			readv(in_rec, mage_spell[py.misc.pclass, i1].learned)
			sscanf(in_rec, '%d', [@ib1]);
			mage_spell[py.misc.pclass, i1].learned:= Boolean(ib1);
			end;
		end
	else
		begin
		for i1:= 1 to 31 do
			with mage_spell[py.misc.pclass, i1] do
				begin
				readln(f1, in_rec);
				decrypt(in_rec);
//				readv(in_rec, learned, sexp)
				sscanf(in_rec, '%d %d', [@ib1, @ib2]);
				learned:= Boolean(ib1);
				sexp:= ib2;
				end;
		for i1:= 1 to 31 do
			with priest_spell[py.misc.pclass, i1] do
				begin
				readln(f1, in_rec);
				decrypt(in_rec);
//				readv(in_rec, learned, sexp)
				sscanf(in_rec, '%d %d', [@ib1, @ib2]);
				learned:= Boolean(ib1);
				sexp:= ib2;
				end;
		for i1:= 1 to 31 do
			with extra_spell[py.misc.pclass, i1] do
				begin
				readln(f1, in_rec);
				decrypt(in_rec);
//				readv(in_rec, learned, sexp)
				sscanf(in_rec, '%d %d-', [@ib1, @ib2]);
				learned:= Boolean(ib1);
				sexp:= ib2;
				end;
		end;

	{ If same version, restore dungeon level... }
	if (not(dun_flag)) then
		begin
		{ Read the important dungeon info and floor }
		readln(f1, in_rec);
		decrypt(in_rec);
//		readv(in_rec, cur_height, cur_width, max_panel_rows, max_panel_cols);
		sscanf(in_rec, '%d %d %d %d', [@ib1, @ib2, @ib3, @ib4]);
		cur_height:= ib1;
		cur_width:= ib2;
		max_panel_rows:= ib3;
		max_panel_cols:= ib4;

//fixme  Why do these values sometimes end up as zero or incredibly large?
		if dun_level > 0 then
			begin
			if  (max_panel_rows <> 4) then
				begin
				MoriaLog(mlrError, 'max_panel_rows <> 4 (' + AnsiString(IntToStr(max_panel_rows)) + ')');
				max_panel_rows:= 4;
				end;

			if  (max_panel_cols <> 4) then
				begin
				MoriaLog(mlrError, 'max_panel_cols <> 4 (' + AnsiString(IntToStr(max_panel_cols)) + ')');
				max_panel_cols:= 4;
				end;
			end
		else
			begin
			if  (max_panel_rows <> 0) then
				begin
				MoriaLog(mlrError, 'max_panel_rows <> 0 (' + AnsiString(IntToStr(max_panel_rows)) + ')');
				max_panel_rows:= 0;
				end;

			if  (max_panel_cols <> 0) then
				begin
				MoriaLog(mlrError, 'max_panel_cols <> 0 (' + AnsiString(IntToStr(max_panel_cols)) + ')');
				max_panel_cols:= 0;
				end;
			end;

		{ Restore the floor }
		for i1:= 1 to cur_height do
			begin
			readln(f1, in_rec);
			decrypt(in_rec);
			for i2:= 1 to cur_width do
				begin
				xfloor:= ord(in_rec[i2]) and $7F;
				with cave[i1, i2] do
					begin
					fval:= {int(uand}($0F and xfloor){)};
					if (($10 and xfloor) <> 0) then
						fopen:= true;
					if (($20 and xfloor) <> 0) then
						pl:= true;
					if (($40 and xfloor) <> 0) then
						fm:= true;
					tl    := false;
					tptr  := 0;
					cptr  := 0;
					end;
				end;
			end;

		if  (not ((cave[char_row, char_col].fopen)
		and  (cave[char_row, char_col].cptr = 0)
		and  (cave[char_row, char_col].tptr = 0)))
		or  (cave[char_row, char_col].fval > 5)
		or  (max_panel_rows <> ib3)
		or  (max_panel_cols <> ib4) then
			begin
			char_col:= -1;
			char_row:= -1;
			end;

		{ Restore the Treasure List }
		tlink;
		readln(f1, in_rec);
		decrypt(in_rec);
//		readv(in_rec, tot_treasures);
		sscanf(in_rec, '%d', [@tot_treasures]);
		for i1:= 1 to tot_treasures do
			begin
			popt(i2);
			with t_list[i2] do
				begin
				readln(f1, in_rec);
				decrypt(in_rec);
//				readv(in_rec, i3, i4);
				sscanf(in_rec, '%d %d', [@i3, @i4]);
				cave[i3, i4].tptr:= i2;

				readln(f1, in_rec);
				decrypt(in_rec);
//				readv(in_rec, tchar, name);
				tchar:= in_rec[1];
				name:= Copy(in_rec, 2, SizeOf(TType));

				readln(f1, in_rec);
				decrypt(in_rec);
//				readv(in_rec, damage);
				damage:= in_rec;

				readln(f1, in_rec);
				decrypt(in_rec);
//				readv(in_rec, tval, subval, weight, number, tohit, todam, ac,
//					toac, p1, flags, level, cost);
				sscanf(in_rec, '%d %d %d %d %d %d %d %d %d %d %d %d', [@ib1,
						@ib2, @ib3, @ib4, @ib5, @ib6, @ib7, @ib8, @ib9, @ib10,
						@ib11, @ib12]);
				tval:= ib1;
				subval:= ib2;
				weight:= ib3;
				number:= ib4;
				tohit:= ib5;
				todam:= ib6;
				ac:= ib7;
				toac:= ib8;
				p1:= ib9;
				flags:= ib10;
				level:= ib11;
				cost:= ib12;
				end;
			end;

		{ Re-identify objects }
		readln(f1, in_rec);
		decrypt(in_rec);
		for i1:= 1 to max_objects do
			if (in_rec[i1] = 'T') then
				identify(object_list[i1])
			else
				object_ident[i1]:= false;

		{ Restore the Monster List }
		mlink;
		readln(f1, in_rec);
		decrypt(in_rec);
//		readv(in_rec, tot_monsters);
		sscanf(in_rec, '%d', [@tot_monsters]);
		i3:= 0;
		for i1:= 1 to tot_monsters do
			begin
			readln(f1, in_rec);
			decrypt(in_rec);
			popm(i2);
			with m_list[i2] do
				begin
//				readv(in_rec, fy, fx, mptr, hp, cspeed, csleep, cdis, ml,
//						confused);

				if old_Flag then
					begin
					sscanf(in_rec, '%d %d %d %d %d %d %d %d %d', [@ib1, @ib2,
							@ib3, @ib4, @ib5, @ib6, @ib7, @ib8, @ib9]);

					ib10:= 0;
					ib11:= 0;
					end
				else
					sscanf(in_rec, '%d %d %d %d %d %d %d %d %d %d %d', [@ib1, @ib2,
							@ib3, @ib4, @ib5, @ib6, @ib7, @ib8, @ib9, @ib10, @ib11]);

				fy:= ib1;
				fx:= ib2;
				mptr:= ib3;
				hp:= ib4;
				cspeed:= ib5;
				csleep:= ib6;
				cdis:= ib7;
				ml:= Boolean(ib8);
				confused:= Boolean(ib9);
				elite:= Boolean(ib10);
				stoleau:= ib11;

				cave[fy, fx].cptr:= i2;
				if (muptr = 0) then
					muptr:= i2
				else
					m_list[i3].nptr:= i2;
				nptr:= 0;
				i3:= i2;
				end;
			end;

		{ Restore the town level stores }
		readln(f1, in_rec);
		decrypt(in_rec);
//		readv(in_rec, town_seed);
		sscanf(in_rec, '%d', [@ib1]);
		town_seed:= ib1;

		for i1:= 1 to max_stores do
			with store[i1] do
				begin
				readln(f1, in_rec);
				decrypt(in_rec);
//				readv(in_rec, i2);
				sscanf(in_rec, '%d', [@i2]);
				store_ctr:= i2;
				{ If not current version then re-outfit the stores }
				if (dun_flag) then
					begin
					i2       := 0;
					store_ctr:= 0;
					end;
				for i3:= 1 to i2 do
					with store_inven[i3].sitem do
						begin
						readln(f1, in_rec);
						decrypt(in_rec);
//						readv(in_rec, store_inven[i3].scost);
						sscanf(in_rec, '%d', [@ib1]);
						store_inven[i3].scost:= ib1;

						readln(f1, in_rec);
						decrypt(in_rec);
//						readv(in_rec, tchar, name);
						tchar:= in_rec[1];
						name:= Copy(in_rec, 2, SizeOf(TType));
						readln(f1, in_rec);
						decrypt(in_rec);
//						readv(in_rec, damage);
						damage:= in_rec;
						readln(f1, in_rec);
						decrypt(in_rec);
//						readv(in_rec, tval, subval, weight, number, tohit,
//								todam, ac, toac, p1, flags, level, cost);
						sscanf(in_rec, '%d %d %d %d %d %d %d %d %d %d %d %d',
								[@ib1, @ib2, @ib3, @ib4, @ib5, @ib6, @ib7, @ib8,
								@ib9, @ib10, @ib11, @ib12]);
						tval:= ib1;
						subval:= ib2;
						weight:= ib3;
						number:= ib4;
						tohit:= ib5;
						todam:= ib6;
						ac:= ib7;
						toac:= ib8;
						p1:= ib9;
						flags:= ib10;
						level:= ib11;
						cost:= ib12;
						end;
				readln(f1, in_rec);
				decrypt(in_rec);
//				readv(in_rec, owner, insult_cur, store_open);
				sscanf(in_rec, '%d %d %d', [@ib1, @ib2, @ib3]);
				owner:= ib1;
				insult_cur:= ib2;
				store_open:= ib3;
				end;
		store_maint;
		end;

//	close(f1, ERROR:= continue);
	CloseFile(f1);
//	open(f1, FILE_NAME:= fnam, record_length:= 1024, history:= OLD,
//			disposition:= delete, ERROR:= continue);
//	close(f1, ERROR:= continue);
	DeleteFile(string(fnam));
//	seed    := get_seed;
	SetSeed(get_seed);
	get_char:= dun_flag;
	end;


{ Wizard command for restoring character                -RAK-   }
//[psect(save$code)]
procedure restore_char;
	var
	i1: integer;
	fnam: vtype;
	in_rec,
	temp: ntype;
	temp_id: packed array [1..70] of AnsiChar;
	f1: text;
	f2: file of key_type;
	flag: boolean;

	procedure do_finddel_key;
(*		var
		i, j: Integer;
		key_rec: Key_Type;
		sl: TStringList;*)

		begin
(*		sl:= TStringList.Create;
		try
			while not Eof(f2) do
				begin
				Read(f2, key_rec);
				sl.AddObject(string(key_rec.file_id), TObject(key_rec.seed));
				end;
			Seek(f2, 0);

			i:= sl.IndexOf(string(temp_id));

			if i > -1 then
				begin
				sl.Delete(i);

				for i:= 0 to sl.Count - 1 do
					begin
//					key_rec.file_id:= AnsiString(sl[i]);
					FillChar(key_rec.file_id[1], 70, #0);
					for j:= 1 to Length(sl[i]) do
						key_rec.file_id[j]:= AnsiChar(sl[i][j]);
					key_rec.seed:= Integer(sl.objects[i]);
					Write(f2, key_rec);
					end;
				end;
//			else
//				data_exception;

			finally
			sl.Free;
			end;*)
		end;

	begin
	prt('Name of file to be restored: ',1,1);
	if (get_string(fnam,1,30,48)) then
		begin
//		priv_switch(1);
//		open (f1,file_name:=fnam,
//				record_length:=1024,history:=old,error:=continue);
		AssignFile(f1, string(fnam));
		Reset(f1);
		if (IOResult <> 0) then
			msg_print('Error opening> '+fnam)
		else
			begin
			flag := true;
//			open(f2,file_name:=moria_mas,
//					access_method:=keyed,organization:=indexed,
//					history:=old,sharing:=readwrite,error:=continue);
			AssignFile(f2, string(MORIA_MAS));
			Rewrite(f2);
			if (IOResult <> 0) then
				begin
//				open (f2,file_name:=moria_mas,
//						access_method:=keyed,organization:=indexed,
//						history:=new,sharing:=readwrite,error:=continue);
//				if (status(f2) <> 0) then
//					begin
					msg_print('MASTER could not be opened.');
					flag := false;
//					end;
				end;

			if (flag) then
				begin
//				reset(f1,error:=continue);
				readln(f1, in_rec {,error:=continue});
//				seed := encrypt_seed2;
				SetSeed(encrypt_seed2);
				decrypt(in_rec);
//				temp := substr(in_rec,14,70);
				temp := Copy(in_rec, 14, 70);
//				seed := encrypt_seed1;
				SetSeed(encrypt_seed1);
				encrypt(temp);
				for i1 := 1 to 70 do
					key_rec.file_id[i1] := temp[i1];
//				temp_id := '     ';
				FillChar(temp_id[1], 70, #0);
				FillChar(temp_id[1], 5, #32);

//				findk(f2,0,temp_id,eql,error:=continue);
//				delete(f2,error:=continue);
				do_finddel_key;

//				f2^ := key_rec;
//				put(f2,error:=continue);
				Write(f2, key_rec);

				if (IOResult = 0) then
					msg_print('Character restored...')
				else
					msg_print('Could not write ID in MASTER.');
				end;
//			close(f1,error:=continue);
			CloseFile(f1);
//			close(f2,error:=continue);
			CloseFile(f2);
			end;
//		seed := get_seed;
		SetSeed(get_seed);
//		priv_switch(0);
		end;
	end;

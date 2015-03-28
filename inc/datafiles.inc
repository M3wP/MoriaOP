
{ Initialize, restore, and get the ball rolling.
  Read in the critters' flags and convert them.  -Opusii}

procedure gethex(var num: unsigned);
	const
	mult = 7;

	var
//	i,
	chnum,
	jindex,
	qcntr: integer;
//	ch: AnsiChar;
//	number: packed array [1 .. 12] of AnsiChar;
	number: string[12];

	begin
	num   := 0;
	qcntr := 0;
	jindex:= 1;
	readln(opusii, number);
	repeat
		if ((number[jindex] >= 'A') and (number[jindex] <= 'F')) then
			chnum:= ord(number[jindex]) - ord('A') + 10
		else
			chnum:= ord(number[jindex]) - ord('0');
		if (number[jindex] = '''') then
			qcntr:= qcntr + 1;
		if (qcntr > 0) and (((jindex - 4) >= 0) and ((jindex - 4) <= 7)) then
			num:= num + (Trunc(Power(16, (mult - (jindex - 4))))) * chnum;
		jindex := jindex + 1;
		until (qcntr = 2);
	end;



procedure read_creatures;
	var
	Opusii_cnt,
	indx: Integer;

	begin
{ read creatures from external file }
//	open(opusii, MORIA_MON, readonly);
	AssignFile(opusii, string(MORIA_MON));
	reset(opusii);
	readln(opusii, max_creatures);
	for Opusii_cnt:= 1 to max_creatures do
		with c_list[Opusii_cnt] do
			begin
			readln(opusii, name);
			for indx:= 1 to 3 do
				begin
				gethex(hexnum);
				case indx of
					1:
						cmove:= hexnum;
					2:
						spells:= hexnum;
					3:
						cdefense:= hexnum
					end
				end;
			readln(opusii, sleep, mexp, aaf, ac, speed);
			readln(opusii, cchar);
			readln(opusii, hd);
			readln(opusii, damage);
			readln(opusii, level);
			end;
//	close(opusii);
	CloseFile(opusii);
	end;


procedure read_data;
	begin
	read_creatures;
	end;


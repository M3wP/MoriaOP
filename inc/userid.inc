{Check userid - allow only certain users to access wizard mode - MWK,Opusii}
//[psect(id$code)]
function valid_user: Boolean;
(*	type
	word    = 0..65535;
	recj    = record
		unameinfo: packed record
			unamelen        : word;
			jpi$_username   : word;
			end;
		ptr_uname       : ^usernam;
		ptr_unamelen    : ^integer;
		ENDlist         : integer
		end;
	usernam = packed array [1..12] of char;*)

(*	var
	mwkusr, curusr: usernam;
	file1 : text;*)

{ function returns the players USERNAME                 -JWT-   }
(*	function mwk_get_username : usernam;
		var
		user            : usernam;
		icode           : integer;
		jpirec          : recj;

{ calls GETJPI routine to return the USERNAME           -JWT-   }
		function sys_getjpi       (%immed p1      : integer;
								%immed  p2      : integer;
								%immed  p3      : integer;
								var     itmlst  : recj;
								%immed  p4      : integer;
								%immed  p5      : integer;
								%immed  p6      : integer) : integer;
				external;

		begin
		with jpirec do
			begin
			unameinfo.unamelen          := 12;
			unameinfo.jpi$_username     := %x202;
			new (ptr_uname);
			ptr_uname^                  := '            ';
			new (ptr_unamelen);
			ptr_unamelen^               := 0;
			ENDlist                     := 0
			end;
		icode := SYS$GETJPI (0,0,0,jpirec,0,0,0);
		if not odd(icode) then
			begin
			writeln('Error in GETJPI process');
			halt
			end
		else
			mwk_get_username := jpirec.ptr_uname^
		end;*)

	begin                                                         {-NBT-}
	valid_user:= False;
//dengland Change this to something a little more difficult
//todo Make wizard mode something of a challenge?
	if ParamCount > 0 then
		if ParamStr(1) = '/WIZARD' then
			valid_user:= True;

(*	mwkusr:= mwk_get_username;
	priv_switch(1);
	open(file1,file_name:=MORIA_WIZ, organization:=sequential,history:=old,
		   sharing:=readwrite,error:=continue);
	if (status(file1) = 0) then
		begin
		reset(file1);
		while not(eof(file1)) do
			begin
			readln(file1,curusr,error:=continue);
			if mwkusr = curusr then
				valid_user := true;
			end;
		close(file1,error:=continue);
		end
	else
		begin
		prt ('ERROR: A severe error occurred in IDSection. Please notify the DM...',1,1);
		Pause(24);
		End;*)
	end;

//[psect(id$code)]
(*function loseruser: Boolean;
	type
	word    = 0..65535;
	recj    = record
		unameinfo       : packed record
			unamelen        : word;
			jpi$_username   : word;
			end;
		ptr_uname       : ^usernam;
		ptr_unamelen    : ^integer;
		ENDlist         : integer
		end;
	usernam         = packed array [1..12] of char;

	var
	mwkusr, curusr: usernam;
	file1 : text;

		{ function returns the players USERNAME                 -JWT-   }
	function mwk_get_username : usernam;
		var
		user            : usernam;
		icode           : integer;
		jpirec          : recj;

		{ calls GETJPI routine to return the USERNAME           -JWT-   }
	  function sys$getjpi       (%immed p1      : integer;
								%immed  p2      : integer;
								%immed  p3      : integer;
								var     itmlst  : recj;
								%immed  p4      : integer;
								%immed  p5      : integer;
								%immed  p6      : integer) : integer;
		external;

	  begin
		with jpirec do
		  begin
			unameinfo.unamelen          := 12;
			unameinfo.jpi$_username     := %x202;
			new (ptr_uname);
			ptr_uname^                  := '            ';
			new (ptr_unamelen);
			ptr_unamelen^               := 0;
			ENDlist                     := 0
		  end;
		icode := SYS$GETJPI (0,0,0,jpirec,0,0,0);
		if not odd(icode) then
		  begin
			writeln('Error in GETJPI process');
			halt
		  end
		else
		  mwk_get_username := jpirec.ptr_uname^
		end;

	begin                                                         {-NBT-}
	loseruser := False;
	mwkusr:=mwk_get_username;
	priv_switch(1);
	open(file1,file_name:=MORIA_LOS, organization:=sequential,history:=old,
		   sharing:=readwrite,error:=continue);
	if (status(file1) = 0) then
		begin
		reset(file1);
		while not(eof(file1)) do
			begin
			readln(file1,curusr,error:=continue);
			if mwkusr = curusr then
				loseruser := true;
			end;
		close(file1,error:=continue);
		end
	else
		begin
		prt ('ERROR: A severe error occurred in IDSection. Please notify the DM...',1,1);
		Pause(24);
		End;
	end;*)

function  GetComputerName: string;
{$IFDEF WINDOWS}
	var
	computerNameBuffer: array[0..255] of Char;
	sizeBuffer: DWord;
{$ENDIF}

	begin
{$IFDEF WINDOWS}
	SizeBuffer:= 256;
	Windows.getComputerName(computerNameBuffer, sizeBuffer);
	Result:= string(computerNameBuffer);
{$ENDIF}

{$IFDEF UNIX}
	Result:= Unix.GetHostName;
{$ENDIF}
	end;


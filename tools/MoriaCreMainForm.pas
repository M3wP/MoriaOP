unit MoriaCreMainForm;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.Menus, Vcl.ComCtrls, Vcl.ActnList,
  Vcl.ToolWin, Vcl.StdCtrls, Math;

{$IFDEF MSWINDOWS}
	{$DEFINE WINDOWS}
{$ENDIF}

{$INCLUDE CONSTANTS.INC}
{$INCLUDE TYPES.INC}

type
  TForm1 = class(TForm)
	lblFileName: TLabel;
	edtFileName: TEdit;
	btnOpenFile: TButton;
	mnmnuMain: TMainMenu;
	tlbStandard: TToolBar;
	actlstMain: TActionList;
	actFileOpen: TAction;
	lstvwCreatures: TListView;
	File1: TMenuItem;
	actFileExit: TAction;
	Exit1: TMenuItem;
	procedure actFileExitExecute(Sender: TObject);
	procedure actFileOpenExecute(Sender: TObject);
	procedure FormCreate(Sender: TObject);
	procedure FormDestroy(Sender: TObject);
  private
	Opusii: Text;
	hexnum: Unsigned;
	max_creatures: Integer;
	FSortList: TStringList;
	c_list : array [1..400] of creature_type;

	procedure gethex(var num: unsigned);
	procedure read_creatures;

	procedure BuildSortListCChar;
	procedure SortCreatures;
	procedure DisplayCreatures;
  public
	{ Public declarations }
  end;

var
  Form1: TForm1;

implementation

{$R *.dfm}

function DoSortCreatureCChar(List: TStringList; Index1, Index2: Integer): Integer;
	begin
	if Form1.c_list[Index1].cchar > Form1.c_list[Index2].cchar then
		Result:= 1
	else if Form1.c_list[Index1].cchar = Form1.c_list[Index2].cchar then
		Result:= 0
	else
		Result:= -1;

//	if Result = 0 then
//		Result:= AnsiStrComp(AnsiString(Form1.c_list[Index1].name),
//				AnsiString(Form1.c_list[Index2].name));
	end;

procedure TForm1.actFileExitExecute(Sender: TObject);
	begin
	Application.Terminate;
	end;

procedure TForm1.actFileOpenExecute(Sender: TObject);
	begin
	read_creatures;
	BuildSortListCChar;
	SortCreatures;
	DisplayCreatures;
	end;

procedure TForm1.BuildSortListCChar;
	var
	i: Integer;

	begin
	FSortList.Clear;

	for i:= 1 to max_creatures do
		FSortList.AddObject(c_list[i].cchar {+ ' ' + c_list[i].name}, Pointer(i));
	end;

procedure TForm1.DisplayCreatures;
	var
	i: Integer;
	itm: TListItem;
	cre: Integer;

	begin
	for i:= 0 to FSortList.Count - 1 do
		begin
		cre:= Integer(FSortList.Objects[i]);
		with c_list[cre] do
			begin
			itm:= lstvwCreatures.Items.Add;
			itm.Caption:= cchar;
			itm.Data:= Pointer(cre);
			itm.SubItems.Add(name);
			end;
		end;
	end;

procedure TForm1.FormCreate(Sender: TObject);
	begin
	FSortList:= TStringList.Create;
	end;

procedure TForm1.FormDestroy(Sender: TObject);
	begin
	FSortList.Free;
	end;

procedure TForm1.gethex(var num: unsigned);
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

procedure TForm1.read_creatures;
	var
	Opusii_cnt,
	indx: Integer;

	begin
{ read creatures from external file }
//	open(opusii, MORIA_MON, readonly);
	AssignFile(opusii, edtFileName.Text);
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

procedure TForm1.SortCreatures;
	begin
//	FSortList.CustomSort(DoSortCreatureCChar);
	FSortList.Duplicates:= dupAccept;
	FSortList.CaseSensitive:= True;
	FSortList.Sort;
	end;

end.

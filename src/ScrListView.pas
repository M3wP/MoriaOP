unit ScrListView;

{$IFDEF FPC}
	{$MODE DELPHI}
{$ENDIF}

interface

uses
	SysUtils, SimpleScreen;

const
	COLR_LIST_TITLE1 = AnsiChar(attrBGWhite + attrFGBlack);
	COLR_LIST_TITLE2 = AnsiChar(attrBGWhite + attrFGRed);
	COLR_LIST_ITMHDR = AnsiChar(attrBGLightGray + attrFGBlack);

type
	TScrListView = class;

	TScrListViewTabCallback = procedure(const AListView: TScrListView;
			const ATab: Integer; const AData: Pointer; var ALabel: AnsiString);
	TScrListViewItemCallback = procedure(const AListView: TScrListView;
			const AScreen: TCustomSimpleScreen; const APosition: TScreenPos;
			const ATab: Integer; const APage: Integer; const AItem: Integer;
			const AData: Pointer);

	TScrListView = class(TObject)
	private
		FScreen: TCustomSimpleScreen;
		FTabCallback: TScrListViewTabCallback;
		FItemCallback: TScrListViewItemCallback;
		FData: Pointer;
		FPosition: TScreenPos;
		FTitle: AnsiString;
		FHeading: AnsiString;
		FTabCount: Integer;
		FPageSize: Integer;
		FItemCount: Integer;
		FCurrentTab: Integer;
		FCurrentPage: Integer;
		FPageCount: Integer;
		FShowCurrent: Boolean;
		FClearEmpty: Boolean;
		FBGColour: TScreenColour;

		procedure DoRecalcPageCount;

		function  GetScreen: TCustomSimpleScreen;
		function  GetPosition: TScreenPos;
		function  GetCurrentPage: Integer;
		function  GetCurrentTab: Integer;
		function  GetHeading: AnsiString;
		function  GetItemCount: Integer;
		function  GetPageSize: Integer;
		function  GetTabCount: Integer;
		function  GetTitle: AnsiString;
		function  GetPageCount: Integer;
		function  GetShowCurrent: Boolean;
		function  GetClearEmpty: Boolean;
		function  GetBGColour: TScreenColour;

		procedure SetCurrentPage(const AValue: Integer);
		procedure SetCurrentTab(const AValue: Integer);
		procedure SetHeading(const AValue: AnsiString);
		procedure SetItemCount(const AValue: Integer);
		procedure SetPageSize(const AValue: Integer);
		procedure SetTabCount(const AValue: Integer);
		procedure SetTitle(const AValue: AnsiString);
		procedure SetPosition(const AValue: TScreenPos);
		procedure SetShowCurrent(const AValue: Boolean);
		procedure SetClearEmpty(const AValue: Boolean);
		procedure SetBGColour(const AValue: TScreenColour);

	public
		constructor Create(const AScreen: TCustomSimpleScreen;
				const ATabCallback: TScrListViewTabCallback;
				const AItemCallback: TScrListViewItemCallback;
				const AData:Pointer);

		procedure Clear;
		procedure Display;

		property  Screen: TCustomSimpleScreen read GetScreen;
		property  Position: TScreenPos read GetPosition write SetPosition;
		property  Title: AnsiString read GetTitle write SetTitle;
		property  Heading: AnsiString read GetHeading write SetHeading;
		property  TabCount: Integer read GetTabCount write SetTabCount;
		property  PageSize: Integer read GetPageSize write SetPageSize;
		property  ItemCount: Integer read GetItemCount write SetItemCount;
		property  CurrentTab: Integer read GetCurrentTab write SetCurrentTab;
		property  CurrentPage: Integer read GetCurrentPage write SetCurrentPage;
		property  PageCount: Integer read GetPageCount;
		property  ShowCurrent: Boolean read GetShowCurrent write SetShowCurrent;
		property  ClearEmpty: Boolean read GetClearEmpty write SetClearEmpty;
		property  BGColour: TScreenColour read GetBGColour write SetBGColour;
	end;


implementation

{ TScrListView }

procedure TScrListView.Clear;
	begin
	FTabCount:= 0;
	FPageCount:= 0;
	FItemCount:= 0;

	FCurrentTab:= -1;
	FCurrentPage:= -1;
	end;

constructor TScrListView.Create(const AScreen: TCustomSimpleScreen;
		const ATabCallback: TScrListViewTabCallback;
		const AItemCallback: TScrListViewItemCallback; const AData:Pointer);
	begin
	inherited Create;

	FScreen:= AScreen;
	FTabCallback:= ATabCallback;
	FItemCallback:= AItemCallback;
	FData:= AData;

	FPosition.X:= 1;
	FPosition.Y:= 1;

	FTitle:= '';
	FHeading:= '';
	FPageSize:= 0;

	Clear;

	FShowCurrent:= True;
	FClearEmpty:= True;
	FBGColour:= sscBlack;
	end;

procedure TScrListView.Display;
	var
	s: AnsiString;
	i,
	w: Integer;
	p: TScreenPos;
	start,
	stop: Integer;

	begin
//	Sanity check
//	if FItemCount = 0 then
//		Exit;

//	Prepare
	w:= FScreen.ScreenWidth - FPosition.X + 1;
	p.X:= FPosition.X;
	p.y:= FPosition.Y;
	s:= '';

//fixme This uses the whole screen width, starting at FPosition.X
	FScreen.BeginUpdate;
	try
//		Display title and '[<tab label> pg <pg>/<pg cnt>]' (if FShowCurrent)
		if  FShowCurrent
		and Assigned(FTabCallback) then
			begin
			FTabCallback(Self, FCurrentTab, FData, s);

			i:= w - 9 - Length(s);
			end
		else
			i:= w;

		FScreen.GotoXY(p.X, p.Y);
		FScreen.ClrEOL;
		FScreen.Write(AnsiString(Copy((FTitle), 1, i)));
		FScreen.PutAttribs(StringOfChar(COLR_LIST_TITLE1, i), p.X, p.Y);

		if  FShowCurrent
		and Assigned(FTabCallback) then
			begin
//fixme		This only handles up to 9 pages
			FScreen.GotoXY(p.X + i, p.Y);
			FScreen.Write('[' + s + ' pg ' +
					AnsiString(IntToStr(FCurrentPage + 1)) + '/' +
					AnsiString(IntToStr(FPageCount)) + ']');
			FScreen.PutAttribs(COLR_LIST_TITLE2 +
					StringOfChar(COLR_LIST_TITLE1, Length(s)) +
					StringOfChar(COLR_LIST_TITLE1, 7) +
					COLR_LIST_TITLE2, p.X + i, p.Y);
			end;

//		Display heading
		Inc(p.Y);
		FScreen.GotoXY(p.X, p.Y);
		FScreen.ClrEOL;
		FScreen.Write(AnsiString(Copy(FHeading, 1, w)));
		FScreen.PutAttribs(StringOfChar(COLR_LIST_ITMHDR, w), p.X, p.Y);

//		Display items
		start:= FCurrentPage * FPageSize;
		stop:= start + FPageSize - 1;
		if stop > (FItemCount - 1) then
			stop:= (FItemCount - 1);

		for i:= start to stop do
			begin
			Inc(p.Y);
			FScreen.GotoXY(p.X, p.Y);
			FScreen.ClrEol;
			FItemCallback(Self, FScreen, p, FCurrentTab, FCurrentPage, i, FData);
			end;

		if FClearEmpty then
			for i:= stop to start + FPageSize - 1 do
				begin
				Inc(p.Y);
				FScreen.GotoXY(p.X, p.Y);
				FScreen.ClrEol;
				FScreen.PutAttribs(StringOfChar(AnsiChar(Ord(FBGColour) shl 4),
						w), p.X, p.Y);
				end;

		finally
		FScreen.EndUpdate;
		end;
	end;

procedure TScrListView.DoRecalcPageCount;
	begin
	if FPageSize > 0 then
		begin
		FPageCount:= FItemCount div FPageSize;
		if  FItemCount > (FPageCount * FPageSize) then
			Inc(FPageCount);

		FCurrentPage:= 0;
		end
	else
		begin
		FPageCount:= 0;
		FCurrentPage:= -1;
		end;
	end;

function TScrListView.GetBGColour: TScreenColour;
	begin
    Result:= FBGColour;
	end;

function TScrListView.GetClearEmpty: Boolean;
	begin
    Result:= FClearEmpty;
	end;

function TScrListView.GetCurrentPage: Integer;
	begin
	Result:= FCurrentPage;
	end;

function TScrListView.GetCurrentTab: Integer;
	begin
	Result:= FCurrentTab;
	end;

function TScrListView.GetHeading: AnsiString;
	begin
	Result:= FHeading;
	end;

function TScrListView.GetItemCount: Integer;
	begin
	Result:= FItemCount;
	end;

function TScrListView.GetPageCount: Integer;
	begin
	Result:= FPageCount;
	end;

function TScrListView.GetPageSize: Integer;
	begin
	Result:= FPageSize;
	end;

function TScrListView.GetPosition: TScreenPos;
	begin
	Result:= FPosition;
	end;

function TScrListView.GetScreen: TCustomSimpleScreen;
	begin
	Result:= FScreen;
	end;

function TScrListView.GetShowCurrent: Boolean;
	begin
	Result:= FShowCurrent;
	end;

function TScrListView.GetTabCount: Integer;
	begin
	Result:= FTabCount;
	end;

function TScrListView.GetTitle: AnsiString;
	begin
	Result:= FTitle;
	end;

procedure TScrListView.SetBGColour(const AValue: TScreenColour);
	begin
	FBGColour:= AValue;
	end;

procedure TScrListView.SetClearEmpty(const AValue: Boolean);
	begin
    FClearEmpty:= AValue;
	end;

procedure TScrListView.SetCurrentPage(const AValue: Integer);
	begin
	if  FItemCount = 0 then
		FCurrentPage:= -1
	else if AValue >= FPageCount then
		FCurrentPage:= 0
	else if AValue < 0 then
		 FCurrentPage:= FPageCount - 1
	else
		FCurrentPage:= AValue;
	end;

procedure TScrListView.SetCurrentTab(const AValue: Integer);
	begin
	if  FTabCount = 0 then
		FCurrentTab:= -1
	else if AValue >= FTabCount then
		FCurrentTab:= 0
	else if AValue < 0 then
		FCurrentTab:= FTabCount - 1
	else
		FCurrentTab:= AValue;
	end;

procedure TScrListView.SetHeading(const AValue: AnsiString);
	begin
	FHeading:= AValue;
	end;

procedure TScrListView.SetItemCount(const AValue: Integer);
	begin
	if  AValue <= 0 then
		FItemCount:= 0
	else
		FItemCount:= AValue;

	DoRecalcPageCount;
	end;

procedure TScrListView.SetPageSize(const AValue: Integer);
	begin
	if  AValue <= 0 then
		FPageSize:= 0
	else
		FPageSize:= AValue;

//fixme  Don't allow to go beyond the end of the screen

	DoRecalcPageCount;
	end;

procedure TScrListView.SetPosition(const AValue: TScreenPos);
	begin
	FPosition:= AValue;
	end;

procedure TScrListView.SetShowCurrent(const AValue: Boolean);
	begin
	FShowCurrent:= AValue;
	end;

procedure TScrListView.SetTabCount(const AValue: Integer);
	begin
	if  AValue <= 0 then
		begin
		FTabCount:= 0;
		FCurrentTab:= -1;
		end
	else
		begin
		FTabCount:= AValue;
		FCurrentTab:= 0;
		end;
	end;

procedure TScrListView.SetTitle(const AValue: AnsiString);
	begin
	FTitle:= AValue;
	end;

end.

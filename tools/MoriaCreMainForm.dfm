object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 528
  ClientWidth = 612
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = mnmnuMain
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object lblFileName: TLabel
    Left = 8
    Top = 51
    Width = 56
    Height = 13
    Caption = 'File Name:  '
  end
  object edtFileName: TEdit
    Left = 88
    Top = 48
    Width = 435
    Height = 21
    TabOrder = 0
    Text = '.\dat\MONSTERIN.DAT'
  end
  object btnOpenFile: TButton
    Left = 529
    Top = 46
    Width = 75
    Height = 25
    Action = actFileOpen
    TabOrder = 1
  end
  object tlbStandard: TToolBar
    Left = 0
    Top = 0
    Width = 612
    Height = 29
    Caption = 'tlbStandard'
    TabOrder = 2
  end
  object lstvwCreatures: TListView
    Left = 8
    Top = 88
    Width = 596
    Height = 432
    Columns = <
      item
      end
      item
        Width = 300
      end
      item
      end
      item
      end>
    ColumnClick = False
    ReadOnly = True
    RowSelect = True
    TabOrder = 3
    ViewStyle = vsReport
  end
  object mnmnuMain: TMainMenu
    Left = 568
    Top = 40
    object File1: TMenuItem
      Caption = '&File'
      object Exit1: TMenuItem
        Action = actFileExit
      end
    end
  end
  object actlstMain: TActionList
    Left = 568
    Top = 88
    object actFileOpen: TAction
      Category = 'File'
      Caption = '&Open'
      OnExecute = actFileOpenExecute
    end
    object actFileExit: TAction
      Category = 'File'
      Caption = 'E&xit'
      OnExecute = actFileExitExecute
    end
  end
end

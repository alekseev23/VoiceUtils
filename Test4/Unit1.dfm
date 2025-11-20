object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Test'
  ClientHeight = 716
  ClientWidth = 935
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Menu = MainMenu1
  TextHeight = 15
  object StatusBar1: TStatusBar
    Left = 0
    Top = 697
    Width = 935
    Height = 19
    Panels = <
      item
        Alignment = taCenter
        Width = 200
      end
      item
        Alignment = taCenter
        Width = 200
      end
      item
        Alignment = taCenter
        Width = 200
      end
      item
        Alignment = taCenter
        Width = 200
      end
      item
        Width = 50
      end>
  end
  object ListBox1: TListBox
    AlignWithMargins = True
    Left = 3
    Top = 3
    Width = 70
    Height = 691
    Align = alLeft
    ItemHeight = 15
    TabOrder = 1
    OnClick = ListBox1Click
    OnDblClick = ListBox1DblClick
  end
  object ListBox2: TListBox
    AlignWithMargins = True
    Left = 888
    Top = 3
    Width = 44
    Height = 691
    Align = alRight
    ItemHeight = 15
    TabOrder = 2
    OnClick = ListBox2Click
    OnDblClick = ListBox2DblClick
  end
  object Chart1: TChart
    AlignWithMargins = True
    Left = 135
    Top = 3
    Width = 747
    Height = 691
    Title.Text.Strings = (
      'TChart')
    View3D = False
    Align = alClient
    TabOrder = 3
    ExplicitLeft = 79
    ExplicitWidth = 803
    DefaultCanvas = 'TGDIPlusCanvas'
    ColorPaletteIndex = 13
  end
  object ListBox3: TListBox
    AlignWithMargins = True
    Left = 79
    Top = 3
    Width = 50
    Height = 691
    Align = alLeft
    ItemHeight = 15
    TabOrder = 4
    OnClick = ListBox3Click
  end
  object MainMenu1: TMainMenu
    Left = 280
    Top = 144
    object N1: TMenuItem
      Caption = #1060#1072#1081#1083
      object N41: TMenuItem
        Caption = #1054#1090#1082#1088#1099#1090#1100' '#1073#1072#1079#1091' '#1089#1087#1077#1082#1090#1088#1086#1074
        OnClick = N41Click
      end
      object N3: TMenuItem
        Caption = #1054#1090#1082#1088#1099#1090#1100' '#1089#1087#1077#1082#1090#1088#1099
        OnClick = N3Click
      end
    end
  end
  object OpenDialog1: TOpenDialog
    Filter = #1041#1072#1079#1072' '#1089#1087#1077#1082#1090#1088#1086#1074' (*.vdb5)|*.vdb5|'#1042#1089#1077' '#1092#1072#1081#1083#1099' (*.*)|*.*'
    Left = 648
    Top = 184
  end
  object OpenDialog2: TOpenDialog
    Filter = #1041#1072#1079#1072' '#1089#1087#1077#1082#1090#1088#1086#1074' (*.wav)|*.wav|'#1042#1089#1077' '#1092#1072#1081#1083#1099' (*.*)|*.*'
    Left = 704
    Top = 328
  end
  object ActionList1: TActionList
    Left = 504
    Top = 152
    object MakeUnvisible: TAction
      Caption = 'MakeUnvisible'
      OnExecute = MakeUnvisibleExecute
    end
    object MakeUnvisible2: TAction
      Caption = 'MakeUnvisible2'
      OnExecute = MakeUnvisible2Execute
    end
  end
end

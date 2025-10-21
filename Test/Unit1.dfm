object Form7: TForm7
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
    Panels = <>
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
  end
  object Chart1: TChart
    AlignWithMargins = True
    Left = 79
    Top = 3
    Width = 803
    Height = 691
    Title.Text.Strings = (
      'TChart')
    View3D = False
    Align = alClient
    TabOrder = 3
    DefaultCanvas = 'TGDIPlusCanvas'
    ColorPaletteIndex = 13
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
    object N7: TMenuItem
      Caption = #1064#1082#1072#1083#1072
      object N5: TMenuItem
        Caption = #1051#1080#1085#1077#1081#1085#1072#1103
        Checked = True
        RadioItem = True
        OnClick = N5Click
      end
      object N6: TMenuItem
        Caption = #1044#1077#1089#1103#1090#1080#1095#1085#1099#1081' '#1083#1086#1075#1072#1088#1080#1092#1084
        RadioItem = True
        OnClick = N6Click
      end
      object X1: TMenuItem
        Caption = #1051#1086#1075#1072#1088#1080#1092#1084' '#1087#1086' '#1086#1089#1085#1086#1074#1072#1085#1080#1102' X'
        RadioItem = True
        OnClick = X1Click
      end
    end
  end
  object OpenDialog1: TOpenDialog
    Filter = #1041#1072#1079#1072' '#1089#1087#1077#1082#1090#1088#1086#1074' (*.vdb)|*.vdb|'#1042#1089#1077' '#1092#1072#1081#1083#1099' (*.*)|*.*'
    Left = 648
    Top = 184
  end
  object OpenDialog2: TOpenDialog
    Filter = #1041#1072#1079#1072' '#1089#1087#1077#1082#1090#1088#1086#1074' (*.dat)|*.dat|'#1042#1089#1077' '#1092#1072#1081#1083#1099' (*.*)|*.*'
    Left = 704
    Top = 328
  end
end

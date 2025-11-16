object Form1: TForm1
  Left = 0
  Top = 0
  Caption = #1055#1088#1086#1089#1084#1086#1090#1088' '#1089#1087#1077#1082#1090#1088#1086#1074' DAT '#1092#1072#1081#1083#1072
  ClientHeight = 970
  ClientWidth = 1046
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Menu = MainMenu1
  TextHeight = 15
  object Splitter1: TSplitter
    Left = 0
    Top = 409
    Width = 1046
    Height = 3
    Cursor = crVSplit
    Align = alTop
    ExplicitLeft = 1
    ExplicitTop = 1
    ExplicitWidth = 495
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 951
    Width = 1046
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
        Width = 50
      end>
  end
  object TrackBar1: TTrackBar
    Left = 0
    Top = 906
    Width = 1046
    Height = 45
    Align = alBottom
    TabOrder = 1
    OnChange = Action1Execute
  end
  object Chart1: TChart
    Left = 0
    Top = 0
    Width = 1046
    Height = 409
    Title.Text.Strings = (
      'N = 512')
    View3D = False
    Align = alTop
    TabOrder = 2
    DefaultCanvas = 'TGDIPlusCanvas'
    ColorPaletteIndex = 13
  end
  object Chart2: TChart
    Left = 0
    Top = 412
    Width = 1046
    Height = 494
    Title.Text.Strings = (
      'N = 8192')
    View3D = False
    Align = alClient
    TabOrder = 3
    DefaultCanvas = 'TGDIPlusCanvas'
    ColorPaletteIndex = 13
  end
  object MainMenu1: TMainMenu
    Left = 880
    Top = 120
    object N1: TMenuItem
      Caption = #1060#1072#1081#1083
      object DAT1: TMenuItem
        Caption = #1054#1090#1082#1088#1099#1090#1100' DAT '#1092#1072#1081#1083
        OnClick = DAT1Click
      end
    end
    object N2: TMenuItem
      Caption = #1064#1077#1081#1087#1077#1088
      object N3: TMenuItem
        Caption = #1055#1088#1103#1084#1086#1091#1075#1086#1083#1100#1085#1099#1081
        Checked = True
        RadioItem = True
        ShortCut = 8304
        OnClick = N3Click
      end
      object N4: TMenuItem
        Caption = #1058#1088#1072#1087#1077#1094#1077#1074#1080#1076#1085#1099#1081
        RadioItem = True
        ShortCut = 8305
        OnClick = N4Click
      end
      object N5: TMenuItem
        Caption = #1053#1086#1088#1084#1072#1083#1100#1085#1099#1081' '#1075#1072#1091#1089#1089#1080#1072#1085
        RadioItem = True
        ShortCut = 8306
        OnClick = N5Click
      end
      object N6: TMenuItem
        Caption = #1059#1079#1082#1080#1081' '#1075#1072#1091#1089#1089#1080#1072#1085
        RadioItem = True
        ShortCut = 8307
        OnClick = N6Click
      end
      object N7: TMenuItem
        Caption = #1064#1080#1088#1086#1082#1080#1081' '#1075#1072#1091#1089#1089#1080#1072#1085
        RadioItem = True
        ShortCut = 8308
        OnClick = N7Click
      end
    end
    object N8: TMenuItem
      Caption = #1044#1080#1072#1087#1072#1079#1086#1085
      object N9: TMenuItem
        Caption = #1064#1080#1088#1086#1082#1080#1081
        Checked = True
        RadioItem = True
        ShortCut = 8314
        OnClick = N9Click
      end
      object N10: TMenuItem
        Caption = #1059#1079#1082#1080#1081
        RadioItem = True
        ShortCut = 8315
        OnClick = N10Click
      end
    end
    object N11: TMenuItem
      Caption = #1055#1088#1080#1083#1086#1078#1077#1085#1080#1103
      object N12: TMenuItem
        Caption = #1047#1072#1087#1091#1089#1090#1080#1090#1100' '#1082#1086#1087#1080#1102
        ShortCut = 16451
        OnClick = N12Click
      end
      object N13: TMenuItem
        Caption = '-'
      end
      object N14: TMenuItem
        Caption = #1055#1077#1088#1077#1079#1072#1087#1091#1089#1090#1080#1090#1100
        ShortCut = 16466
        OnClick = N14Click
      end
    end
  end
  object OpenDialog1: TOpenDialog
    Filter = #1060#1072#1081#1083#1099' '#1089' '#1072#1091#1076#1080#1086' '#1076#1072#1085#1085#1099#1084#1080' (*.dat)|*.dat'
    Left = 808
    Top = 56
  end
  object ActionList1: TActionList
    Left = 520
    Top = 192
    object Action1: TAction
      Caption = 'Action1'
      OnExecute = Action1Execute
    end
  end
end

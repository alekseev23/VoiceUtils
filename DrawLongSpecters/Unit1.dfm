object Form1: TForm1
  Left = 0
  Top = 0
  Caption = #1055#1088#1086#1089#1084#1086#1090#1088' '#1076#1083#1080#1085#1085#1099#1093' '#1089#1087#1077#1082#1090#1088#1086#1074
  ClientHeight = 862
  ClientWidth = 947
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
    Top = 843
    Width = 947
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
        Width = 50
      end>
    ExplicitLeft = 480
    ExplicitTop = 440
    ExplicitWidth = 0
  end
  object TrackBar1: TTrackBar
    Left = 0
    Top = 767
    Width = 947
    Height = 35
    Align = alBottom
    TabOrder = 1
    OnChange = Action1Execute
    ExplicitTop = 808
  end
  object Chart1: TChart
    Left = 0
    Top = 0
    Width = 947
    Height = 767
    Title.Text.Strings = (
      'TChart')
    View3D = False
    Align = alClient
    TabOrder = 2
    ExplicitLeft = 280
    ExplicitTop = 336
    ExplicitWidth = 400
    ExplicitHeight = 250
    DefaultCanvas = 'TGDIPlusCanvas'
    ColorPaletteIndex = 13
  end
  object Panel1: TPanel
    Left = 0
    Top = 802
    Width = 947
    Height = 41
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 3
    ExplicitLeft = -8
    ExplicitTop = 813
    object Button1: TButton
      AlignWithMargins = True
      Left = 3
      Top = 3
      Width = 75
      Height = 35
      Align = alLeft
      Caption = '<<'
      TabOrder = 0
      OnClick = Button1Click
      ExplicitTop = 6
    end
    object Button2: TButton
      AlignWithMargins = True
      Left = 84
      Top = 3
      Width = 75
      Height = 35
      Align = alLeft
      Caption = '>>'
      TabOrder = 1
      OnClick = Button2Click
      ExplicitLeft = 448
      ExplicitTop = 16
      ExplicitHeight = 25
    end
  end
  object MainMenu1: TMainMenu
    Left = 536
    Top = 168
    object N1: TMenuItem
      Caption = #1060#1072#1081#1083
      object N2: TMenuItem
        Caption = #1054#1090#1082#1088#1099#1090#1100
        OnClick = N2Click
      end
      object N3: TMenuItem
        Caption = '-'
      end
      object N4: TMenuItem
        Caption = #1042#1099#1093#1086#1076
        OnClick = N4Click
      end
    end
    object N5: TMenuItem
      Caption = #1055#1088#1080#1083#1086#1078#1077#1085#1080#1103
      object N6: TMenuItem
        Caption = #1047#1072#1087#1091#1089#1090#1080#1090#1100' '#1082#1086#1087#1080#1102
        ShortCut = 16451
        OnClick = N6Click
      end
      object N7: TMenuItem
        Caption = '-'
      end
      object N8: TMenuItem
        Caption = #1055#1077#1088#1077#1079#1072#1087#1091#1089#1090#1080#1090#1100
        ShortCut = 16466
        OnClick = N8Click
      end
    end
    object N9: TMenuItem
      Caption = #1044#1080#1072#1087#1072#1079#1086#1085
      object N14: TMenuItem
        Caption = #1059#1079#1082#1080#1081' (16384)'
        RadioItem = True
        OnClick = N14Click
      end
      object N15: TMenuItem
        Caption = #1057#1088#1077#1076#1085#1080#1081' (32768)'
        RadioItem = True
        OnClick = N15Click
      end
      object N655361: TMenuItem
        Caption = #1064#1080#1088#1086#1082#1080#1081' (65536)'
        Checked = True
        RadioItem = True
        OnClick = N655361Click
      end
    end
    object N10: TMenuItem
      Caption = #1064#1077#1081#1087#1077#1088
      object N16: TMenuItem
        Caption = #1055#1088#1103#1084#1086#1091#1075#1086#1083#1100#1085#1099#1081
        Checked = True
        RadioItem = True
        ShortCut = 8304
        OnClick = N16Click
      end
      object N17: TMenuItem
        Caption = #1058#1088#1072#1087#1077#1094#1077#1074#1080#1076#1085#1099#1081
        RadioItem = True
        ShortCut = 8305
        OnClick = N17Click
      end
      object N18: TMenuItem
        Caption = #1053#1086#1088#1084#1072#1083#1100#1085#1099#1081' '#1043#1072#1091#1089#1089#1080#1072#1085
        RadioItem = True
        ShortCut = 8306
        OnClick = N18Click
      end
      object N19: TMenuItem
        Caption = #1059#1079#1082#1080#1081' '#1043#1072#1091#1089#1089#1080#1072#1085
        RadioItem = True
        ShortCut = 8307
        OnClick = N19Click
      end
      object N20: TMenuItem
        Caption = #1064#1080#1088#1086#1082#1080#1081' '#1043#1072#1091#1089#1089#1080#1072#1085
        RadioItem = True
        ShortCut = 8308
        OnClick = N20Click
      end
    end
    object N11: TMenuItem
      Caption = #1043#1088#1072#1092#1080#1082#1080
      object N12: TMenuItem
        Caption = #1044#1086#1073#1072#1074#1080#1090#1100' 1'
        ShortCut = 112
        OnClick = N12Click
      end
      object N21: TMenuItem
        Caption = #1044#1086#1073#1072#1074#1080#1090#1100' 2'
        ShortCut = 113
        OnClick = N21Click
      end
      object N31: TMenuItem
        Caption = #1044#1086#1073#1072#1074#1080#1090#1100' 3'
        ShortCut = 114
        OnClick = N31Click
      end
      object N41: TMenuItem
        Caption = #1044#1086#1073#1072#1074#1080#1090#1100' 4'
        ShortCut = 115
        OnClick = N41Click
      end
      object N51: TMenuItem
        Caption = #1044#1086#1073#1072#1074#1080#1090#1100' 5'
        ShortCut = 116
        OnClick = N51Click
      end
      object N32: TMenuItem
        Caption = '-'
      end
      object N13: TMenuItem
        Caption = #1054#1095#1080#1089#1090#1080#1090#1100' '#1074#1089#1077
        OnClick = N13Click
      end
    end
    object N22: TMenuItem
      Caption = #1057#1087#1077#1082#1090#1088#1099
      object N23: TMenuItem
        Caption = #1054#1073#1099#1095#1085#1099#1077
        Checked = True
        RadioItem = True
        OnClick = N23Click
      end
      object N24: TMenuItem
        Caption = #1069#1085#1077#1088#1075#1080#1090#1080#1095#1077#1089#1082#1080#1077
        RadioItem = True
        OnClick = N24Click
      end
    end
  end
  object OpenDialog1: TOpenDialog
    Filter = #1060#1072#1081#1083#1099' '#1089' '#1072#1091#1076#1080#1086' '#1076#1072#1085#1085#1099#1084#1080' (*.dat)|*.dat|'#1042#1089#1077' '#1092#1072#1081#1083#1099' (*.*)|*.*'
    Left = 704
    Top = 168
  end
  object ActionList1: TActionList
    Left = 368
    Top = 160
    object Action1: TAction
      Caption = 'Action1'
      OnExecute = Action1Execute
    end
  end
end

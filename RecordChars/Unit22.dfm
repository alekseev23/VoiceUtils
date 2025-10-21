object Form22: TForm22
  Left = 0
  Top = 0
  Caption = #1047#1072#1087#1080#1089#1100' '#1079#1074#1091#1082#1086#1074
  ClientHeight = 702
  ClientWidth = 959
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
    Top = 683
    Width = 959
    Height = 19
    Panels = <>
  end
  object Panel1: TPanel
    Left = 0
    Top = 642
    Width = 959
    Height = 41
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    object BitBtn1: TBitBtn
      AlignWithMargins = True
      Left = 3
      Top = 3
      Width = 75
      Height = 35
      Align = alLeft
      Caption = #1047#1072#1087#1080#1089#1100
      TabOrder = 0
    end
    object BitBtn2: TBitBtn
      AlignWithMargins = True
      Left = 84
      Top = 3
      Width = 75
      Height = 35
      Align = alLeft
      Caption = #1059#1076#1072#1083#1080#1090#1100
      TabOrder = 1
    end
    object BitBtn3: TBitBtn
      AlignWithMargins = True
      Left = 165
      Top = 3
      Width = 100
      Height = 35
      Align = alLeft
      Caption = #1059#1076#1072#1083#1080#1090#1100' '#1074#1089#1077
      TabOrder = 2
    end
    object BitBtn4: TBitBtn
      AlignWithMargins = True
      Left = 271
      Top = 3
      Width = 75
      Height = 35
      Align = alLeft
      Caption = 'BitBtn4'
      TabOrder = 3
    end
  end
  object TabControl2: TTabControl
    Left = 0
    Top = 0
    Width = 959
    Height = 642
    Align = alClient
    TabOrder = 2
    TabPosition = tpBottom
    Tabs.Strings = (
      #1040#1083#1077#1082#1089#1072#1085#1076#1088
      #1058#1072#1090#1100#1103#1085#1072
      #1040#1083#1077#1082#1089#1077#1081
      #1040#1088#1089#1077#1085#1080#1081)
    TabIndex = 0
    object TabControl1: TTabControl
      Left = 4
      Top = 4
      Width = 951
      Height = 614
      Align = alClient
      MultiLine = True
      TabOrder = 0
      Tabs.Strings = (
        #1040
        #1041
        #1042
        #1043
        #1044
        #1045
        #1025
        #1046
        #1047
        #1048
        #1049
        #1050
        #1051
        #1052
        #1053
        #1054
        #1055
        #1056
        #1057
        #1058
        #1059
        #1060
        #1061
        #1062
        #1063
        #1064
        #1065
        #1067
        #1069
        #1070
        #1071)
      TabIndex = 0
      object GroupBox1: TGroupBox
        AlignWithMargins = True
        Left = 7
        Top = 49
        Width = 218
        Height = 558
        Align = alLeft
        Caption = 'GroupBox1'
        TabOrder = 0
        ExplicitTop = 29
        ExplicitHeight = 578
        object ListBox1: TListBox
          AlignWithMargins = True
          Left = 5
          Top = 20
          Width = 208
          Height = 533
          Align = alClient
          ItemHeight = 15
          TabOrder = 0
          ExplicitHeight = 553
        end
      end
      object Chart1: TChart
        Left = 228
        Top = 46
        Width = 719
        Height = 564
        Title.Text.Strings = (
          'TChart')
        View3D = False
        Align = alClient
        TabOrder = 1
        ExplicitTop = 26
        ExplicitHeight = 584
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
      end
    end
  end
  object MainMenu1: TMainMenu
    Left = 472
    Top = 360
    object N1: TMenuItem
      Caption = #1060#1072#1081#1083
    end
  end
end

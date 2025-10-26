object Form1: TForm1
  Left = 0
  Top = 0
  Caption = #1047#1072#1087#1080#1089#1100' '#1079#1074#1091#1082#1086#1074
  ClientHeight = 712
  ClientWidth = 959
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Menu = MainMenu1
  Position = poDesktopCenter
  TextHeight = 15
  object StatusBar1: TStatusBar
    Left = 0
    Top = 693
    Width = 959
    Height = 19
    Panels = <
      item
        Width = 300
      end
      item
        Width = 300
      end
      item
        Width = 50
      end>
  end
  object Panel1: TPanel
    Left = 0
    Top = 652
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
      OnClick = BitBtn1Click
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
      OnClick = BitBtn2Click
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
      OnClick = BitBtn3Click
    end
    object BitBtn4: TBitBtn
      AlignWithMargins = True
      Left = 840
      Top = 3
      Width = 116
      Height = 35
      Align = alRight
      Caption = #1055#1086#1089#1090#1088#1086#1080#1090#1100' '#1074#1089#1077
      TabOrder = 3
      OnClick = BitBtn4Click
    end
    object BitBtn5: TBitBtn
      AlignWithMargins = True
      Left = 704
      Top = 3
      Width = 130
      Height = 35
      Align = alRight
      Caption = #1057#1088#1072#1074#1085#1077#1085#1080#1077' '#1089#1087#1077#1082#1090#1088#1086#1074
      TabOrder = 4
      OnClick = BitBtn5Click
    end
  end
  object TabControl1: TTabControl
    Left = 0
    Top = 52
    Width = 959
    Height = 600
    Align = alClient
    MultiLine = True
    TabOrder = 2
    Tabs.Strings = (
      #1040#1083#1077#1082#1089#1072#1085#1076#1088
      #1058#1072#1090#1100#1103#1085#1072)
    TabIndex = 0
    object GroupBox1: TGroupBox
      AlignWithMargins = True
      Left = 7
      Top = 29
      Width = 50
      Height = 564
      Align = alLeft
      Caption = #1047#1074#1091#1082#1080
      TabOrder = 0
      object ListBox1: TListBox
        AlignWithMargins = True
        Left = 5
        Top = 20
        Width = 40
        Height = 539
        Align = alClient
        ItemHeight = 15
        Items.Strings = (
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
        TabOrder = 0
        OnClick = ListBox1Click
      end
    end
    object GroupBox2: TGroupBox
      AlignWithMargins = True
      Left = 63
      Top = 29
      Width = 58
      Height = 564
      Align = alLeft
      Caption = #1047#1072#1087#1080#1089#1080
      TabOrder = 1
      object ListBox2: TListBox
        AlignWithMargins = True
        Left = 5
        Top = 20
        Width = 48
        Height = 539
        Align = alClient
        ItemHeight = 15
        TabOrder = 0
        OnClick = ListBox2Click
      end
    end
    object Panel2: TPanel
      Left = 124
      Top = 26
      Width = 831
      Height = 570
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 2
      object Splitter1: TSplitter
        Left = 0
        Top = 193
        Width = 831
        Height = 3
        Cursor = crVSplit
        Align = alTop
        ExplicitLeft = 1
        ExplicitTop = 1
        ExplicitWidth = 175
      end
      object PageControl1: TPageControl
        Left = 0
        Top = 0
        Width = 831
        Height = 193
        ActivePage = TabSheet2
        Align = alTop
        TabOrder = 0
        object TabSheet1: TTabSheet
          Caption = #1054#1075#1080#1073#1072#1102#1097#1072#1103
          object Image1: TImage
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 817
            Height = 157
            Align = alClient
            ExplicitLeft = 360
            ExplicitTop = 32
            ExplicitWidth = 105
            ExplicitHeight = 105
          end
        end
        object TabSheet2: TTabSheet
          Caption = #1040#1091#1076#1080#1086#1076#1072#1085#1085#1099#1077
          ImageIndex = 1
          object Chart1: TChart
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 817
            Height = 157
            Legend.Visible = False
            Title.Text.Strings = (
              #1040#1091#1076#1080#1086' '#1076#1072#1085#1085#1099#1077)
            View3D = False
            Align = alClient
            TabOrder = 0
            DefaultCanvas = 'TGDIPlusCanvas'
            ColorPaletteIndex = 13
          end
        end
      end
      object PageControl2: TPageControl
        Left = 0
        Top = 196
        Width = 831
        Height = 374
        ActivePage = TabSheet3
        Align = alClient
        TabOrder = 1
        object TabSheet3: TTabSheet
          Caption = #1042#1089#1077' '#1089#1087#1077#1082#1090#1088#1099
          object TabControl2: TTabControl
            Left = 0
            Top = 0
            Width = 823
            Height = 344
            Align = alClient
            MultiLine = True
            TabOrder = 0
            TabPosition = tpLeft
            Tabs.Strings = (
              #1057#1088#1077#1076#1085#1077#1077
              #1053#1072#1095#1072#1083#1086
              #1057#1077#1088#1077#1076#1080#1085#1072
              #1050#1086#1085#1077#1094
              #1042#1089#1077)
            TabIndex = 0
            OnChange = TabControl2Change
            object Chart2: TChart
              AlignWithMargins = True
              Left = 30
              Top = 7
              Width = 786
              Height = 330
              Legend.Visible = False
              Title.Text.Strings = (
                #1057#1087#1077#1082#1090#1088#1099' '#1086#1090' '#1074#1088#1077#1084#1077#1085#1080' ('#1086#1090' '#1079#1077#1083#1105#1085#1086#1075#1086' '#1082' '#1089#1080#1085#1077#1084#1091')')
              View3D = False
              Align = alClient
              TabOrder = 0
              DefaultCanvas = 'TGDIPlusCanvas'
              ColorPaletteIndex = 13
            end
          end
        end
        object TabSheet7: TTabSheet
          Caption = #1057#1088#1077#1076#1085#1080#1077' '#1089#1087#1077#1082#1090#1088#1099
          ImageIndex = 4
          object Chart4: TChart
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 817
            Height = 338
            Title.Text.Strings = (
              #1057#1088#1077#1076#1085#1080#1077' '#1089#1087#1077#1082#1090#1088#1099)
            View3D = False
            Align = alClient
            TabOrder = 0
            DefaultCanvas = 'TGDIPlusCanvas'
            ColorPaletteIndex = 13
          end
        end
        object TabSheet4: TTabSheet
          Caption = '3D'
          ImageIndex = 1
          object ScrollBox1: TScrollBox
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 817
            Height = 338
            Align = alClient
            TabOrder = 0
            object Image2: TImage
              Left = 0
              Top = 0
              Width = 793
              Height = 321
            end
          end
        end
        object TabSheet5: TTabSheet
          Caption = #1057#1087#1077#1082#1090#1088#1086#1075#1088#1072#1084#1084#1072
          ImageIndex = 2
          object ScrollBox2: TScrollBox
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 817
            Height = 338
            Align = alClient
            TabOrder = 0
            ExplicitLeft = 392
            ExplicitTop = 303
            ExplicitWidth = 185
            ExplicitHeight = 41
            object Image3: TImage
              AlignWithMargins = True
              Left = 0
              Top = 0
              Width = 105
              Height = 105
              Stretch = True
            end
          end
        end
        object TabSheet6: TTabSheet
          Caption = #1048#1079#1084#1077#1085#1077#1085#1080#1103' '#1089#1087#1077#1082#1090#1088#1086#1074
          ImageIndex = 3
          object Chart3: TChart
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 817
            Height = 338
            Title.Text.Strings = (
              #1048#1079#1084#1077#1085#1077#1085#1080#1103' '#1089#1087#1077#1082#1090#1088#1086#1074)
            View3D = False
            Align = alClient
            TabOrder = 0
            DefaultCanvas = 'TGDIPlusCanvas'
            ColorPaletteIndex = 13
          end
        end
      end
    end
  end
  object GroupBox3: TGroupBox
    AlignWithMargins = True
    Left = 3
    Top = 3
    Width = 953
    Height = 46
    Align = alTop
    Caption = #1059#1089#1090#1088#1086#1081#1089#1090#1074#1086' '#1079#1072#1087#1080#1089#1080
    TabOrder = 3
    object ComboBox1: TComboBox
      AlignWithMargins = True
      Left = 5
      Top = 20
      Width = 943
      Height = 23
      Align = alTop
      Style = csDropDownList
      TabOrder = 0
      OnChange = ComboBox1Change
    end
  end
  object MainMenu1: TMainMenu
    Left = 472
    Top = 360
    object N1: TMenuItem
      Caption = #1060#1072#1081#1083
      object N11: TMenuItem
        Caption = #1042#1099#1093#1086#1076
        OnClick = N11Click
      end
    end
    object N25: TMenuItem
      Caption = #1055#1088#1080#1083#1086#1078#1077#1085#1080#1103
      object N13: TMenuItem
        Caption = #1047#1072#1087#1091#1089#1090#1080#1090#1100' '#1082#1086#1087#1080#1102
        ShortCut = 16451
        OnClick = N13Click
      end
      object N14: TMenuItem
        Caption = '-'
      end
      object N15: TMenuItem
        Caption = #1055#1077#1088#1077#1079#1072#1087#1091#1089#1090#1080#1090#1100
        ShortCut = 16466
        OnClick = N15Click
      end
    end
    object N222: TMenuItem
      Caption = #1043#1086#1083#1086#1089#1072
      object N3: TMenuItem
        Caption = #1044#1086#1073#1072#1074#1080#1090#1100
        ShortCut = 16462
        OnClick = N3Click
      end
      object N4: TMenuItem
        Caption = #1059#1076#1072#1083#1080#1090#1100
        ShortCut = 40968
        OnClick = N4Click
      end
      object N5: TMenuItem
        Caption = #1059#1076#1072#1083#1080#1090#1100' '#1074#1089#1077
        OnClick = N5Click
      end
      object N6: TMenuItem
        Caption = '-'
      end
      object N7: TMenuItem
        Caption = #1054#1073#1085#1086#1074#1080#1090#1100
        ShortCut = 16469
        OnClick = UpdateVoicesListExecute
      end
    end
    object N224: TMenuItem
      Caption = #1047#1074#1091#1082
      object N18: TMenuItem
        Caption = #1055#1088#1086#1080#1075#1088#1072#1090#1100' '#1087#1086#1074#1090#1086#1088#1085#1086
        ShortCut = 16464
        OnClick = N18Click
      end
    end
    object N8: TMenuItem
      Caption = #1056#1072#1079#1084#1077#1088#1099' '#1089#1087#1077#1082#1090#1088#1086#1074
      object N2561: TMenuItem
        Caption = '256'
        Checked = True
        RadioItem = True
        OnClick = N2561Click
      end
      object N2562: TMenuItem
        Caption = '512'
        RadioItem = True
        OnClick = N2562Click
      end
      object N10241: TMenuItem
        Caption = '1024'
        RadioItem = True
        OnClick = N10241Click
      end
      object N10242: TMenuItem
        Caption = '2048'
        RadioItem = True
        OnClick = N10242Click
      end
      object N40961: TMenuItem
        Caption = '4096'
        RadioItem = True
        OnClick = N40961Click
      end
    end
    object N200: TMenuItem
      Caption = #1064#1082#1072#1083#1072
      object N9: TMenuItem
        Caption = #1051#1080#1085#1077#1081#1085#1072#1103
        Checked = True
        RadioItem = True
        OnClick = N9Click
      end
      object N10: TMenuItem
        Caption = #1044#1077#1089#1103#1090#1080#1095#1085#1099#1081' '#1083#1086#1075#1072#1088#1080#1092#1084
        RadioItem = True
        OnClick = N10Click
      end
      object X1: TMenuItem
        Caption = #1051#1086#1075#1072#1088#1080#1092#1084' '#1087#1086' '#1086#1089#1085#1086#1074#1072#1085#1080#1102' X'
        RadioItem = True
        OnClick = X1Click
      end
      object N221: TMenuItem
        Caption = '-'
      end
      object N19: TMenuItem
        Caption = #1054#1073#1099#1095#1085#1099#1077' '#1089#1087#1077#1082#1090#1088#1099
        Checked = True
        GroupIndex = 1
        RadioItem = True
        OnClick = N19Click
      end
      object N20: TMenuItem
        Caption = #1069#1085#1077#1088#1075#1077#1090#1080#1095#1077#1089#1082#1080#1077' '#1089#1087#1077#1082#1090#1088#1099
        GroupIndex = 1
        RadioItem = True
        OnClick = N20Click
      end
    end
    object N223: TMenuItem
      Caption = #1042#1080#1076
      object N12: TMenuItem
        Caption = #1057#1087#1080#1089#1086#1082' '#1089#1087#1077#1082#1090#1088#1086#1074
        OnClick = N12Click
      end
      object N28: TMenuItem
        Caption = #1040#1085#1072#1083#1080#1079' '#1084#1072#1082#1089#1080#1084#1091#1084#1086#1074
        OnClick = N28Click
      end
    end
    object N26: TMenuItem
      Caption = #1054#1087#1077#1088#1072#1094#1080#1080
      object N16: TMenuItem
        Caption = #1054#1073#1093#1086#1076' '#1074#1089#1077#1093' '#1074#1072#1088#1080#1072#1085#1090#1086#1074
        OnClick = N16Click
      end
      object N29: TMenuItem
        Caption = '-'
      end
      object N17: TMenuItem
        Caption = #1054#1073#1088#1072#1073#1086#1090#1082#1072' '#1089#1087#1077#1082#1090#1088#1086#1074
        OnClick = N17Click
      end
    end
  end
  object ActionList1: TActionList
    Left = 632
    Top = 248
    object UpdateVoicesList: TAction
      Caption = 'UpdateVoicesList'
      OnExecute = UpdateVoicesListExecute
    end
    object UpdateAudioInputList: TAction
      Caption = 'UpdateAudioInputList'
      OnExecute = UpdateAudioInputListExecute
    end
    object UpdateRecordsList: TAction
      Caption = 'UpdateRecordsList'
      OnExecute = UpdateRecordsListExecute
    end
  end
  object Timer1: TTimer
    OnTimer = Timer1Timer
    Left = 696
    Top = 400
  end
end

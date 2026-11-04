object Form14: TForm14
  Left = 0
  Top = 0
  Caption = #1042#1089#1077' '#1089#1087#1077#1082#1090#1088#1099' '#1074#1089#1077#1093' '#1079#1074#1091#1082#1086#1074
  ClientHeight = 651
  ClientWidth = 847
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  TextHeight = 15
  object ListBox1: TListBox
    AlignWithMargins = True
    Left = 3
    Top = 3
    Width = 30
    Height = 604
    Align = alLeft
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
    MultiSelect = True
    TabOrder = 0
    OnClick = ListBox1Click
  end
  object Panel1: TPanel
    Left = 0
    Top = 610
    Width = 847
    Height = 41
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    ExplicitLeft = 336
    ExplicitTop = 328
    ExplicitWidth = 185
    object BitBtn1: TBitBtn
      AlignWithMargins = True
      Left = 3
      Top = 3
      Width = 102
      Height = 35
      Align = alLeft
      Caption = #1042#1099#1076#1077#1083#1080#1090#1100' '#1074#1089#1077
      TabOrder = 0
      OnClick = BitBtn1Click
    end
    object BitBtn2: TBitBtn
      AlignWithMargins = True
      Left = 111
      Top = 3
      Width = 114
      Height = 35
      Align = alLeft
      Caption = #1054#1090#1084#1077#1085#1080#1090#1100' '#1074#1099#1073#1086#1088
      TabOrder = 1
      OnClick = BitBtn2Click
    end
  end
  object PageControl1: TPageControl
    AlignWithMargins = True
    Left = 39
    Top = 3
    Width = 805
    Height = 604
    ActivePage = TabSheet1
    Align = alClient
    TabOrder = 2
    ExplicitLeft = 288
    ExplicitTop = 248
    ExplicitWidth = 289
    ExplicitHeight = 193
    object TabSheet1: TTabSheet
      Caption = 'TabSheet1'
      object Chart1: TChart
        AlignWithMargins = True
        Left = 3
        Top = 3
        Width = 791
        Height = 568
        Title.Text.Strings = (
          'TChart')
        View3D = False
        Align = alClient
        TabOrder = 0
        ExplicitLeft = 55
        ExplicitWidth = 789
        ExplicitHeight = 604
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'TabSheet2'
      ImageIndex = 1
      object Chart2: TChart
        AlignWithMargins = True
        Left = 3
        Top = 3
        Width = 791
        Height = 568
        Title.Text.Strings = (
          'TChart')
        View3D = False
        Align = alClient
        TabOrder = 0
        ExplicitLeft = 6
        ExplicitTop = 6
        ExplicitWidth = 275
        ExplicitHeight = 157
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
      end
    end
  end
  object ActionList1: TActionList
    Left = 416
    Top = 328
    object Draw: TAction
      Caption = 'Draw'
      OnExecute = DrawExecute
    end
    object Calculate: TAction
      Caption = 'Calculate'
      OnExecute = CalculateExecute
    end
  end
end

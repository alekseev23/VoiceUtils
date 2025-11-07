object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Show Specters'
  ClientHeight = 609
  ClientWidth = 984
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
    Top = 590
    Width = 984
    Height = 19
    Panels = <>
    ExplicitLeft = 480
    ExplicitTop = 296
    ExplicitWidth = 0
  end
  object PageControl1: TPageControl
    AlignWithMargins = True
    Left = 3
    Top = 3
    Width = 978
    Height = 584
    ActivePage = TabSheet1
    Align = alClient
    TabOrder = 1
    ExplicitLeft = 336
    ExplicitTop = 224
    ExplicitWidth = 289
    ExplicitHeight = 193
    object TabSheet1: TTabSheet
      Caption = 'VDB2'
      object Chart1: TChart
        AlignWithMargins = True
        Left = 79
        Top = 3
        Width = 888
        Height = 548
        Title.Text.Strings = (
          'TChart')
        View3D = False
        Align = alClient
        TabOrder = 0
        ExplicitLeft = 3
        ExplicitWidth = 919
        ExplicitHeight = 515
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
      end
      object GroupBox1: TGroupBox
        AlignWithMargins = True
        Left = 3
        Top = 3
        Width = 70
        Height = 548
        Align = alLeft
        Caption = #1057#1087#1077#1082#1090#1088#1099
        TabOrder = 1
        ExplicitLeft = 11
        ExplicitTop = 6
        ExplicitHeight = 515
        object ListBox1: TListBox
          AlignWithMargins = True
          Left = 5
          Top = 20
          Width = 60
          Height = 523
          Align = alClient
          ItemHeight = 15
          TabOrder = 0
          OnClick = ListBox1Click
          ExplicitHeight = 490
        end
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'VDB3'
      ImageIndex = 1
      object ScrollBox1: TScrollBox
        AlignWithMargins = True
        Left = 79
        Top = 3
        Width = 888
        Height = 548
        Align = alClient
        TabOrder = 0
        ExplicitLeft = 368
        ExplicitTop = 240
        ExplicitWidth = 185
        ExplicitHeight = 41
        object Image1: TImage
          Left = 0
          Top = 0
          Width = 105
          Height = 105
        end
      end
      object GroupBox2: TGroupBox
        AlignWithMargins = True
        Left = 3
        Top = 3
        Width = 70
        Height = 548
        Align = alLeft
        Caption = #1057#1087#1077#1082#1090#1088#1099
        TabOrder = 1
        ExplicitLeft = 11
        ExplicitTop = 6
        ExplicitHeight = 515
        object ListBox2: TListBox
          AlignWithMargins = True
          Left = 5
          Top = 20
          Width = 60
          Height = 523
          Align = alClient
          ItemHeight = 15
          TabOrder = 0
          OnClick = ListBox2Click
          ExplicitLeft = 7
          ExplicitTop = 22
          ExplicitHeight = 490
        end
      end
    end
  end
  object MainMenu1: TMainMenu
    Left = 568
    Top = 160
    object N1: TMenuItem
      Caption = #1060#1072#1081#1083
      object vdb21: TMenuItem
        Caption = #1054#1090#1082#1088#1099#1090#1100' '#1089#1087#1077#1082#1090#1088#1099' *.vdb2'
        OnClick = vdb21Click
      end
      object vdb31: TMenuItem
        Caption = #1054#1090#1082#1088#1099#1090#1100' '#1089#1087#1077#1082#1090#1088#1099' *.vdb3'
        OnClick = vdb31Click
      end
      object vdb32: TMenuItem
        Caption = '-'
      end
      object N23: TMenuItem
        Caption = #1042#1099#1093#1086#1076
        OnClick = N23Click
      end
    end
    object N3: TMenuItem
      Caption = #1055#1088#1080#1083#1086#1078#1077#1085#1080#1103
      object N4: TMenuItem
        Caption = #1047#1072#1087#1091#1089#1090#1080#1090#1100' '#1082#1086#1087#1080#1102
        ShortCut = 16451
        OnClick = N4Click
      end
      object N5: TMenuItem
        Caption = '-'
      end
      object N6: TMenuItem
        Caption = #1055#1077#1088#1077#1079#1072#1087#1091#1089#1090#1080#1090#1100
        ShortCut = 16466
        OnClick = N6Click
      end
    end
    object N24: TMenuItem
      Caption = #1048#1085#1089#1090#1088#1091#1084#1077#1085#1090#1099
      object N7: TMenuItem
        Caption = #1055#1086#1083#1086#1089#1099' '#1095#1072#1089#1090#1086#1090
        OnClick = N7Click
      end
    end
  end
  object OpenDialog1: TOpenDialog
    Filter = 'VDB2|*.vdb2'
    Left = 672
    Top = 160
  end
  object OpenDialog2: TOpenDialog
    Filter = 'VDB3|*.vdb3'
    Left = 776
    Top = 160
  end
end

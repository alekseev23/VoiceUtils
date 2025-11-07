object Form8: TForm8
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #1040#1074#1090#1086#1084#1072#1090#1080#1095#1077#1089#1082#1080#1081' '#1086#1073#1093#1086#1076' '#1079#1074#1091#1082#1086#1074
  ClientHeight = 473
  ClientWidth = 509
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poMainFormCenter
  OnCloseQuery = FormCloseQuery
  TextHeight = 15
  object Panel1: TPanel
    Left = 0
    Top = 413
    Width = 509
    Height = 41
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 0
    ExplicitTop = 250
    object BitBtn1: TBitBtn
      AlignWithMargins = True
      Left = 269
      Top = 3
      Width = 75
      Height = 35
      Align = alRight
      Caption = #1057#1090#1072#1088#1090
      TabOrder = 0
      OnClick = BitBtn1Click
    end
    object BitBtn2: TBitBtn
      AlignWithMargins = True
      Left = 350
      Top = 3
      Width = 75
      Height = 35
      Align = alRight
      Caption = #1057#1090#1086#1087
      TabOrder = 1
      OnClick = BitBtn2Click
    end
    object BitBtn3: TBitBtn
      AlignWithMargins = True
      Left = 431
      Top = 3
      Width = 75
      Height = 35
      Align = alRight
      Caption = #1047#1072#1082#1088#1099#1090#1100
      TabOrder = 2
      OnClick = BitBtn3Click
    end
  end
  object GroupBox1: TGroupBox
    AlignWithMargins = True
    Left = 3
    Top = 79
    Width = 503
    Height = 49
    Align = alTop
    Caption = #1043#1086#1083#1086#1089#1072
    TabOrder = 1
    object ProgressBar1: TProgressBar
      AlignWithMargins = True
      Left = 5
      Top = 20
      Width = 493
      Height = 17
      Align = alTop
      TabOrder = 0
    end
  end
  object GroupBox2: TGroupBox
    AlignWithMargins = True
    Left = 3
    Top = 134
    Width = 503
    Height = 49
    Align = alTop
    Caption = #1047#1074#1091#1082#1080
    TabOrder = 2
    object ProgressBar2: TProgressBar
      AlignWithMargins = True
      Left = 5
      Top = 20
      Width = 493
      Height = 17
      Align = alTop
      TabOrder = 0
    end
  end
  object GroupBox3: TGroupBox
    AlignWithMargins = True
    Left = 3
    Top = 189
    Width = 503
    Height = 49
    Align = alTop
    Caption = #1042#1072#1088#1080#1072#1085#1090#1099
    TabOrder = 3
    object ProgressBar3: TProgressBar
      AlignWithMargins = True
      Left = 5
      Top = 20
      Width = 493
      Height = 17
      Align = alTop
      TabOrder = 0
    end
  end
  object GroupBox4: TGroupBox
    AlignWithMargins = True
    Left = 3
    Top = 3
    Width = 503
    Height = 70
    Align = alTop
    Caption = #1055#1072#1088#1072#1084#1077#1090#1088#1099
    TabOrder = 4
    object CheckBox1: TCheckBox
      Left = 16
      Top = 32
      Width = 97
      Height = 17
      Caption = #1042#1089#1077' '#1075#1086#1083#1086#1089#1072
      TabOrder = 0
    end
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 454
    Width = 509
    Height = 19
    Panels = <
      item
        Alignment = taCenter
        Width = 150
      end
      item
        Alignment = taCenter
        Width = 150
      end
      item
        Alignment = taCenter
        Width = 150
      end
      item
        Width = 50
      end>
    ExplicitTop = 291
  end
  object GroupBox5: TGroupBox
    AlignWithMargins = True
    Left = 3
    Top = 244
    Width = 503
    Height = 166
    Align = alTop
    Caption = #1044#1077#1083#1072#1090#1100' '#1089#1082#1088#1080#1085#1096#1086#1090#1099
    TabOrder = 6
    object CheckBox2: TCheckBox
      AlignWithMargins = True
      Left = 5
      Top = 20
      Width = 493
      Height = 17
      Align = alTop
      Caption = #1040#1091#1076#1080#1086' '#1076#1072#1085#1085#1099#1077
      TabOrder = 0
      ExplicitLeft = 200
      ExplicitTop = 48
      ExplicitWidth = 97
    end
    object CheckBox3: TCheckBox
      AlignWithMargins = True
      Left = 5
      Top = 43
      Width = 493
      Height = 17
      Align = alTop
      Caption = #1042#1089#1077' '#1089#1087#1077#1082#1090#1088#1099' '#1076#1072#1085#1085#1086#1075#1086' '#1079#1074#1091#1082#1072
      TabOrder = 1
      ExplicitLeft = 7
      ExplicitTop = 60
    end
    object CheckBox4: TCheckBox
      AlignWithMargins = True
      Left = 5
      Top = 66
      Width = 493
      Height = 17
      Align = alTop
      Caption = #1042#1089#1077' '#1089#1087#1077#1082#1090#1088#1099
      TabOrder = 2
      ExplicitLeft = 7
      ExplicitTop = 89
    end
    object CheckBox5: TCheckBox
      AlignWithMargins = True
      Left = 5
      Top = 89
      Width = 493
      Height = 17
      Align = alTop
      Caption = #1057#1088#1077#1076#1085#1080#1077' '#1089#1087#1077#1082#1090#1088#1099
      TabOrder = 3
      ExplicitLeft = 3
      ExplicitTop = 112
    end
    object CheckBox6: TCheckBox
      AlignWithMargins = True
      Left = 5
      Top = 112
      Width = 493
      Height = 17
      Align = alTop
      Caption = '3D'
      TabOrder = 4
      ExplicitLeft = 3
      ExplicitTop = 131
    end
    object CheckBox7: TCheckBox
      AlignWithMargins = True
      Left = 5
      Top = 135
      Width = 493
      Height = 17
      Align = alTop
      Caption = #1057#1087#1077#1082#1090#1088#1086#1075#1088#1072#1084#1084#1072
      TabOrder = 5
      ExplicitLeft = 3
      ExplicitTop = 149
    end
  end
  object Timer1: TTimer
    Enabled = False
    OnTimer = Timer1Timer
    Left = 344
    Top = 32
  end
end

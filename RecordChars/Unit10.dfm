object Form10: TForm10
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #1056#1072#1079#1084#1077#1088' '#1089#1087#1077#1082#1090#1088#1086#1075#1088#1072#1084#1084#1099
  ClientHeight = 116
  ClientWidth = 165
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poMainFormCenter
  TextHeight = 15
  object Panel1: TPanel
    Left = 0
    Top = 75
    Width = 165
    Height = 41
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 0
    object BitBtn1: TBitBtn
      AlignWithMargins = True
      Left = 6
      Top = 3
      Width = 75
      Height = 35
      Align = alRight
      Caption = #1057#1073#1088#1086#1089
      TabOrder = 0
      OnClick = BitBtn1Click
    end
    object BitBtn3: TBitBtn
      AlignWithMargins = True
      Left = 87
      Top = 3
      Width = 75
      Height = 35
      Align = alRight
      Caption = #1047#1072#1082#1088#1099#1090#1100
      TabOrder = 1
      OnClick = BitBtn3Click
    end
  end
  object StaticText1: TStaticText
    Left = 16
    Top = 12
    Width = 11
    Height = 19
    Caption = 'X'
    TabOrder = 1
  end
  object ComboBox1: TComboBox
    Left = 48
    Top = 8
    Width = 81
    Height = 23
    Style = csDropDownList
    TabOrder = 2
    OnClick = ApplyExecute
    Items.Strings = (
      '1 / 20'
      '1 / 10'
      '1 / 5'
      '1 / 3'
      '1 / 2'
      '1'
      '3 /2'
      '2'
      '3'
      '4'
      '5'
      '10')
  end
  object ComboBox2: TComboBox
    Left = 48
    Top = 37
    Width = 81
    Height = 23
    Style = csDropDownList
    TabOrder = 3
    OnClick = ApplyExecute
    Items.Strings = (
      '1 / 20'
      '1 / 10'
      '1 / 5'
      '1 / 3'
      '1 / 2'
      '1'
      '3 /2'
      '2'
      '3'
      '4'
      '5'
      '10')
  end
  object StaticText2: TStaticText
    Left = 16
    Top = 41
    Width = 11
    Height = 19
    Caption = 'Y'
    TabOrder = 4
  end
  object ActionList1: TActionList
    Left = 120
    Top = 24
    object Apply: TAction
      Caption = 'Apply'
      OnExecute = ApplyExecute
    end
  end
end

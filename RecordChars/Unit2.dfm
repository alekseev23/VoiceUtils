object Form2: TForm2
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #1044#1086#1073#1072#1074#1083#1077#1085#1080#1077' '#1075#1086#1083#1086#1089#1072
  ClientHeight = 117
  ClientWidth = 288
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
    Top = 76
    Width = 288
    Height = 41
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 0
    object BitBtn1: TBitBtn
      AlignWithMargins = True
      Left = 129
      Top = 3
      Width = 75
      Height = 35
      Align = alRight
      Caption = #1044#1086#1073#1072#1074#1080#1090#1100
      TabOrder = 0
      OnClick = BitBtn1Click
    end
    object BitBtn2: TBitBtn
      AlignWithMargins = True
      Left = 210
      Top = 3
      Width = 75
      Height = 35
      Align = alRight
      Caption = #1047#1072#1082#1088#1099#1090#1100
      TabOrder = 1
      OnClick = BitBtn2Click
    end
  end
  object GroupBox1: TGroupBox
    AlignWithMargins = True
    Left = 3
    Top = 3
    Width = 282
    Height = 70
    Align = alClient
    Caption = #1042#1074#1077#1076#1080#1090#1077' '#1080#1084#1103' '#1082#1072#1090#1072#1083#1086#1075#1072
    TabOrder = 1
    object Edit1: TEdit
      AlignWithMargins = True
      Left = 5
      Top = 20
      Width = 272
      Height = 23
      Align = alTop
      TabOrder = 0
      TextHint = #1042#1074#1077#1076#1080#1090#1077' '#1080#1084#1103
    end
  end
end

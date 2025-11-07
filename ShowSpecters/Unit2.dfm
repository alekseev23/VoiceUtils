object Form2: TForm2
  Left = 0
  Top = 0
  Caption = #1055#1086#1083#1086#1089#1099' '#1095#1072#1089#1090#1086#1090
  ClientHeight = 596
  ClientWidth = 863
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  TextHeight = 15
  object StatusBar1: TStatusBar
    Left = 0
    Top = 577
    Width = 863
    Height = 19
    Panels = <>
    ExplicitLeft = 432
    ExplicitTop = 336
    ExplicitWidth = 0
  end
  object Panel1: TPanel
    Left = 0
    Top = 536
    Width = 863
    Height = 41
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    ExplicitLeft = 344
    ExplicitTop = 328
    ExplicitWidth = 185
    object BitBtn1: TBitBtn
      AlignWithMargins = True
      Left = 548
      Top = 3
      Width = 100
      Height = 35
      Align = alRight
      Caption = #1054#1095#1080#1089#1090#1080#1090#1100
      TabOrder = 0
      OnClick = BitBtn1Click
      ExplicitLeft = 560
    end
    object BitBtn2: TBitBtn
      AlignWithMargins = True
      Left = 654
      Top = 3
      Width = 100
      Height = 35
      Align = alRight
      Caption = #1050#1086#1087#1080#1088#1086#1074#1072#1090#1100
      TabOrder = 1
      OnClick = BitBtn2Click
      ExplicitLeft = 666
    end
    object BitBtn3: TBitBtn
      AlignWithMargins = True
      Left = 760
      Top = 3
      Width = 100
      Height = 35
      Align = alRight
      Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
      TabOrder = 2
      ExplicitLeft = 772
    end
    object BitBtn4: TBitBtn
      AlignWithMargins = True
      Left = 3
      Top = 3
      Width = 100
      Height = 35
      Align = alLeft
      Caption = #1054#1095#1080#1089#1090#1080#1090#1100
      TabOrder = 3
      OnClick = BitBtn4Click
    end
    object BitBtn5: TBitBtn
      AlignWithMargins = True
      Left = 109
      Top = 3
      Width = 100
      Height = 35
      Align = alLeft
      Caption = #1057#1073#1088#1086#1089#1080#1090#1100
      TabOrder = 4
      OnClick = BitBtn5Click
      ExplicitLeft = 84
    end
  end
  object StringGrid1: TStringGrid
    AlignWithMargins = True
    Left = 3
    Top = 3
    Width = 209
    Height = 530
    Align = alLeft
    ColCount = 3
    DefaultColWidth = 60
    RowCount = 11
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing, goFixedRowDefAlign]
    TabOrder = 2
    OnExit = StringGrid1Exit
    ExplicitLeft = 0
    ExplicitTop = 0
    ExplicitHeight = 590
  end
  object Memo1: TMemo
    AlignWithMargins = True
    Left = 218
    Top = 3
    Width = 642
    Height = 530
    Align = alClient
    Lines.Strings = (
      'Memo1')
    TabOrder = 3
    ExplicitLeft = 344
    ExplicitTop = 304
    ExplicitWidth = 185
    ExplicitHeight = 89
  end
end

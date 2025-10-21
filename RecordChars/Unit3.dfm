object Form3: TForm3
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #1054#1090#1086#1073#1088#1072#1078#1072#1077#1084#1099#1077' '#1089#1087#1077#1082#1090#1088#1099
  ClientHeight = 677
  ClientWidth = 166
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poMainFormCenter
  TextHeight = 15
  object ListBox1: TListBox
    AlignWithMargins = True
    Left = 3
    Top = 3
    Width = 160
    Height = 671
    Align = alClient
    ItemHeight = 15
    MultiSelect = True
    PopupMenu = PopupMenu1
    TabOrder = 0
    OnClick = ListBox1Click
    ExplicitHeight = 651
  end
  object PopupMenu1: TPopupMenu
    Left = 88
    Top = 432
    object N1: TMenuItem
      Caption = #1042#1099#1073#1088#1072#1090#1100' '#1074#1089#1077
    end
    object N2: TMenuItem
      Caption = #1054#1090#1084#1077#1085#1080#1090#1100' '#1074#1089#1077
    end
  end
end

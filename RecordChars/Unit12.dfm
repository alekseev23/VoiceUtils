object Form12: TForm12
  Left = 0
  Top = 0
  Caption = #1053#1080#1078#1085#1080#1077' '#1089#1087#1077#1082#1090#1088#1099
  ClientHeight = 712
  ClientWidth = 1026
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  TextHeight = 15
  object Chart1: TChart
    Left = 41
    Top = 0
    Width = 985
    Height = 712
    Title.Text.Strings = (
      'TChart')
    View3D = False
    Align = alClient
    TabOrder = 0
    ExplicitLeft = 320
    ExplicitTop = 248
    ExplicitWidth = 400
    ExplicitHeight = 250
    DefaultCanvas = 'TGDIPlusCanvas'
    ColorPaletteIndex = 13
  end
  object ListBox1: TListBox
    Left = 0
    Top = 0
    Width = 41
    Height = 712
    Align = alLeft
    ItemHeight = 15
    MultiSelect = True
    TabOrder = 1
    OnClick = DrawExecute
  end
  object ActionList1: TActionList
    Left = 728
    Top = 224
    object Draw: TAction
      Caption = 'Draw'
      OnExecute = DrawExecute
    end
  end
end

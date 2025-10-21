object Form6: TForm6
  Left = 0
  Top = 0
  Caption = #1057#1088#1072#1074#1085#1077#1085#1080#1077' '#1089#1087#1077#1082#1090#1088#1086#1074
  ClientHeight = 607
  ClientWidth = 838
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  TextHeight = 15
  object StatusBar1: TStatusBar
    Left = 0
    Top = 588
    Width = 838
    Height = 19
    Panels = <>
    ExplicitLeft = 424
    ExplicitTop = 312
    ExplicitWidth = 0
  end
  object PageControl1: TPageControl
    AlignWithMargins = True
    Left = 3
    Top = 3
    Width = 832
    Height = 582
    ActivePage = TabSheet2
    Align = alClient
    TabOrder = 1
    ExplicitLeft = 280
    ExplicitTop = 224
    ExplicitWidth = 289
    ExplicitHeight = 193
    object TabSheet1: TTabSheet
      Caption = 'TabSheet1'
      object Memo1: TMemo
        AlignWithMargins = True
        Left = 3
        Top = 3
        Width = 818
        Height = 546
        Align = alClient
        Lines.Strings = (
          'Memo1')
        ScrollBars = ssBoth
        TabOrder = 0
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'TabSheet2'
      ImageIndex = 1
      object Chart1: TChart
        AlignWithMargins = True
        Left = 3
        Top = 3
        Width = 818
        Height = 546
        Title.Text.Strings = (
          'TChart')
        View3D = False
        Align = alClient
        TabOrder = 0
        ExplicitLeft = -56
        ExplicitTop = -40
        ExplicitWidth = 400
        ExplicitHeight = 250
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object Series1: TBarSeries
          HoverElement = []
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Bar'
          YValues.Order = loNone
        end
      end
    end
  end
  object ActionList1: TActionList
    Left = 552
    Top = 208
    object Analyze: TAction
      Caption = 'Analyze'
      OnExecute = AnalyzeExecute
    end
  end
end

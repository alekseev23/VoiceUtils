object Form5: TForm5
  Left = 0
  Top = 0
  Caption = 'Form5'
  ClientHeight = 441
  ClientWidth = 624
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  TextHeight = 15
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 624
    Height = 441
    ActivePage = TabSheet1
    Align = alClient
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = 'TabSheet1'
      object Image1: TImage
        AlignWithMargins = True
        Left = 3
        Top = 3
        Width = 496
        Height = 405
        Align = alClient
        Center = True
        Proportional = True
        Stretch = True
        ExplicitLeft = 272
        ExplicitTop = 192
        ExplicitWidth = 105
        ExplicitHeight = 105
      end
      object Image2: TImage
        AlignWithMargins = True
        Left = 508
        Top = 3
        Width = 105
        Height = 405
        Align = alRight
        Stretch = True
        ExplicitLeft = 256
        ExplicitTop = 152
        ExplicitHeight = 105
      end
      object Splitter1: TSplitter
        Left = 502
        Top = 0
        Height = 411
        Align = alRight
        ExplicitLeft = 304
        ExplicitTop = 152
        ExplicitHeight = 100
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'TabSheet2'
      ImageIndex = 1
      object Memo1: TMemo
        AlignWithMargins = True
        Left = 3
        Top = 3
        Width = 610
        Height = 405
        Align = alClient
        Lines.Strings = (
          'Memo1')
        ScrollBars = ssBoth
        TabOrder = 0
      end
    end
    object TabSheet3: TTabSheet
      Caption = 'TabSheet3'
      ImageIndex = 2
      object Memo2: TMemo
        AlignWithMargins = True
        Left = 3
        Top = 3
        Width = 610
        Height = 405
        Align = alClient
        Lines.Strings = (
          'Memo2')
        ScrollBars = ssBoth
        TabOrder = 0
      end
    end
  end
end

object Cell3D: TCell3D
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = 'Cell3D'
  ClientHeight = 282
  ClientWidth = 587
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu
  OldCreateOrder = False
  OnPaint = FormPaint
  OnResize = FormPaint
  PixelsPerInch = 96
  TextHeight = 13
  object MainMenu: TMainMenu
    Left = 416
    Top = 112
    object Zoom: TMenuItem
      Caption = 'Zoom(1)'
      object Zoom1: TMenuItem
        Caption = 'Zoom=1'
        Checked = True
        RadioItem = True
        OnClick = Zoom1Click
      end
      object Zoom2: TMenuItem
        Caption = 'Zoom=2'
        RadioItem = True
        OnClick = Zoom2Click
      end
      object Zoom3: TMenuItem
        Caption = 'Zoom=3'
        RadioItem = True
        OnClick = Zoom3Click
      end
      object Zoom6: TMenuItem
        Caption = 'Zoom=6'
        RadioItem = True
        OnClick = Zoom6Click
      end
      object Zoom12: TMenuItem
        Caption = 'Zoom=12'
        RadioItem = True
        OnClick = Zoom12Click
      end
    end
    object Tilt: TMenuItem
      Caption = 'Tilt(0.6)'
      OnClick = TiltClick
    end
    object Swing: TMenuItem
      Caption = 'Swing(0.5)'
      OnClick = SwingClick
    end
    object SlicePosition: TMenuItem
      Caption = 'Slice(550nm)'
      OnClick = SlicePositionClick
    end
    object SliceTilt: TMenuItem
      Caption = 'SliceTilt(0.2)'
      OnClick = SliceTiltClick
    end
    object SaveBMP: TMenuItem
      Caption = 'SaveBMP'
      OnClick = SaveBMPClick
    end
    object VertShift: TMenuItem
      Caption = 'VertShift(500)'
      OnClick = VertShiftClick
    end
    object BuildTube: TMenuItem
      Caption = 'BuildTubeOFF'
      OnClick = BuildTubeClick
    end
  end
  object SaveBmpDlg: TSavePictureDialog
    DefaultExt = 'bmp'
    Filter = 'Bitmaps (*.bmp)|*.bmp'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofEnableSizing]
    Left = 480
    Top = 112
  end
end

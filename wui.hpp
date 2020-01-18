// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Magic.
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#pragma once
#pragma comment(lib, "Comctl32.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Imports.
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include <map>
#include <array>
#include <string>
#define NOMINMAX
#include <windows.h>
#include <commctrl.h>

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Windows UI.
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
namespace wui
{
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Version.
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	constexpr auto VERSION_MAJOR = int(1);
	constexpr auto VERSION_MINOR = int(1);
	constexpr auto VERSION_PATCH = int(0);

	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Globals.
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool IsInit = false;
	WNDCLASSEX WindowClass;
	HINSTANCE InstanceHandle;
	struct Control;
	std::map<HWND, Control*> Controls;

	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Bitmap entry.
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	struct BitmapEntry
	{
		HBITMAP Handle;
		void* Data;
		int Width;
		int Height;
		int Depth;

		BitmapEntry ( void ) : Handle(NULL), Data(NULL), Width(0), Height(0), Depth(0) {}
	};

	std::map<std::string, BitmapEntry> Bitmaps;

	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Create bitmap for use in controls. BGR 8bit per color. Channel remapping is done by user.
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	auto createBitmap ( const std::string& _Name, const int _Width, const int _Height ) -> void
	{
		if(Bitmaps.count(_Name) == 0)
		{
			auto DeviceContext = CreateCompatibleDC(NULL);

			auto BitmapInfo = BITMAPINFO();
			ZeroMemory(&BitmapInfo.bmiHeader, sizeof(BITMAPINFOHEADER));

			BitmapInfo.bmiHeader.biWidth = _Width;
			BitmapInfo.bmiHeader.biHeight = -_Height; // Invert height to shift 0 to top left.
			BitmapInfo.bmiHeader.biPlanes = 1; // Not channels.
			BitmapInfo.bmiHeader.biBitCount = 24; // 3 Channel bitmap. BGR color layout.
			BitmapInfo.bmiHeader.biSizeImage = 0;
			BitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			BitmapInfo.bmiHeader.biClrUsed = 0;
			BitmapInfo.bmiHeader.biClrImportant = 0;

			auto Entry = BitmapEntry();
			Entry.Handle = CreateDIBSection(DeviceContext, &BitmapInfo, DIB_RGB_COLORS, &Entry.Data, NULL, 0);

			DeleteDC(DeviceContext);

			if(Entry.Handle != NULL)
			{
				Entry.Width = _Width;
				Entry.Height = _Height;
				Entry.Depth = 3;

				Bitmaps[_Name] = Entry;
			}
			
			else
			{
				auto Message = std::string("Failed to create bitmap: ") + _Name + std::string("! - CreateDIBSection() returned NULL.");
				MessageBox(NULL, Message.c_str(), "WUI", MB_ICONERROR | MB_OK);
			}
		}

		else
		{
			auto Message = std::string("Failed to create bitmap: ") + _Name + std::string("! - Name collision.");
			MessageBox(NULL, Message.c_str(), "WUI", MB_ICONERROR | MB_OK);
		}
	}

	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Update bitmap's data.
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	auto updateBitmap ( const std::string& _Bitmap, const void* _Data ) -> void
	{
		auto Bitmap = Bitmaps.find(_Bitmap);
			
		if(Bitmap != Bitmaps.end())
		{
			std::memcpy(Bitmap->second.Data, _Data, size_t(Bitmap->second.Width * Bitmap->second.Height * Bitmap->second.Depth));
		}

		else
		{
			auto Message = std::string("Failed to update bitmap: ") + _Bitmap + std::string("! - Name not found.");
			MessageBox(NULL, Message.c_str(), "WUI", MB_ICONERROR | MB_OK);
		}
	}

	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Get bitmap's entry.
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	auto getBitmap ( const std::string& _Bitmap ) -> BitmapEntry&
	{
		auto Bitmap = Bitmaps.find(_Bitmap);
			
		if(Bitmap != Bitmaps.end())
		{
			return Bitmap->second;
		}

		else
		{
			auto Message = std::string("Failed to get bitmap: ") + _Bitmap + std::string("! - Name not found.");
			MessageBox(NULL, Message.c_str(), "WUI", MB_ICONERROR | MB_OK);
		}
	}

	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Delete bitmap.
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	auto killBitmap ( const std::string& _Bitmap ) -> void
	{
		auto Bitmap = Bitmaps.find(_Bitmap);
			
		if(Bitmap != Bitmaps.end())
		{
			DeleteObject(Bitmap->second.Handle);
			Bitmaps.erase(_Bitmap);
		}

		else
		{
			auto Message = std::string("Failed to kill bitmap: ") + _Bitmap + std::string("! - Name not found.");
			MessageBox(NULL, Message.c_str(), "WUI", MB_ICONERROR | MB_OK);
		}
	}

	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Enum for control type.
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	enum struct ControlType
	{
		ROOT,
		WINDOW,

		BUTTON,
		CHECKBOX,
		GROUPBOX,
		RADIOBUTTON,

		OUTPUTBOX,
		INPUTBOX,

		TEXT,
		IMAGE
	};

	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Enum for horizontal aligment.
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	enum struct AlignH
	{
		LEFT,
		RIGHT,
		CENTER
	};

	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Object to describe window traits.
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	struct WindowTraits
	{
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Window traits.
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		bool IsChild; // For child windows.
		bool IsTitled; // Enable title bar.
		bool IsResizeble; // Enable window resizing.
		bool IsAlwaysOnTop; // Enable window to be always on top.
		bool IsToolWindow; // Enable tool window. Smaller title bar.

		bool HasSysMenu; // Enable system(min, max, close) menu.
		bool HasBorder; // Enable window border.
		bool HasHScrollBar; // Enable horizontal scrollbar.
		bool HasVScrollBar; // Enable vertical scrollbar.

		bool CanBeMinimized; // Enable minimization. Needs system menu enabled.
		bool CanBeMaximized; // Enable maximization. Needs system menu enabled.
		bool CanAcceptFiles; // Enable drag and drop files onto window.

		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Default constructor.
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		WindowTraits ( void ) :
			IsChild(false),
			IsTitled(false),
			IsResizeble(false),
			IsAlwaysOnTop(false),
			IsToolWindow(false),
			HasSysMenu(false),
			HasBorder(false),
			HasHScrollBar(false),
			HasVScrollBar(false),
			CanBeMinimized(false),
			CanBeMaximized(false),
			CanAcceptFiles(false){}
		
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Build window style.
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto buildStyle ( void ) const -> unsigned long
		{
			auto Style = unsigned long(0);

			if(this->IsChild) Style |= WS_CHILD;
			if(this->IsTitled) Style |= WS_CAPTION;
			if(this->IsResizeble) Style |= WS_SIZEBOX;
			if(this->HasSysMenu) Style |= WS_SYSMENU;
			if(this->HasBorder) Style |= WS_BORDER;
			if(this->HasHScrollBar) Style |= WS_HSCROLL;
			if(this->HasVScrollBar) Style |= WS_VSCROLL;
			if(this->CanBeMinimized) Style |= WS_MINIMIZEBOX;
			if(this->CanBeMaximized) Style |= WS_MAXIMIZEBOX;

			return Style;
		}

		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Build extended window style.
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto buildStyleEx ( void ) const -> unsigned long
		{
			auto Style = unsigned long(0);

			if(this->IsAlwaysOnTop) Style |= WS_EX_TOPMOST;
			if(this->IsToolWindow) Style |= WS_EX_TOOLWINDOW;
			if(this->CanAcceptFiles) Style |= WS_EX_ACCEPTFILES;

			return Style;
		}
	};

	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Get traits for standard window.
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	auto getWindowTraitsStandard ( void ) -> WindowTraits
	{
		auto Traits = WindowTraits();
			
		Traits.IsChild = false;
		Traits.IsTitled = true;
		Traits.IsResizeble = true;
		Traits.IsAlwaysOnTop = false;
		Traits.IsToolWindow = false;
		Traits.HasSysMenu = true;
		Traits.HasBorder = true;
		Traits.HasHScrollBar = false;
		Traits.HasVScrollBar = false;
		Traits.CanBeMinimized = true;
		Traits.CanBeMaximized = true;
		Traits.CanAcceptFiles = false;
		
		return Traits;
	}

	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Get traits for standard window that cannot be resized and has just close button.
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	auto getWindowTraitsFixed ( void ) -> WindowTraits
	{
		auto Traits = WindowTraits();
			
		Traits.IsChild = false;
		Traits.IsTitled = true;
		Traits.IsResizeble = false;
		Traits.IsAlwaysOnTop = false;
		Traits.IsToolWindow = false;
		Traits.HasSysMenu = true;
		Traits.HasBorder = true;
		Traits.HasHScrollBar = false;
		Traits.HasVScrollBar = false;
		Traits.CanBeMinimized = false;
		Traits.CanBeMaximized = false;
		Traits.CanAcceptFiles = false;
		
		return Traits;
	}

	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Get traits for window with border only.
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	auto getWindowTraitsPanel ( void ) -> WindowTraits
	{
		auto Traits = WindowTraits();
			
		Traits.IsChild = false;
		Traits.IsTitled = false;
		Traits.IsResizeble = false;
		Traits.IsAlwaysOnTop = false;
		Traits.IsToolWindow = false;
		Traits.HasSysMenu = false;
		Traits.HasBorder = true;
		Traits.HasHScrollBar = false;
		Traits.HasVScrollBar = false;
		Traits.CanBeMinimized = false;
		Traits.CanBeMaximized = false;
		Traits.CanAcceptFiles = false;
		
		return Traits;
	}

	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Wrapper for windows window.
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	struct Control
	{
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Members.
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		std::string Name;
		HWND Handle;
		Control* Root;
		ControlType Type;
		std::map<std::string, Control*> Children;
		
		bool IsClosed;
		bool IsClicked;

		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Default constructor. Create root window.
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		Control ( void )
		{
			this->Name = "WuiRootWindow";
			this->Type = ControlType::ROOT;
		}
		
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Explicit constructor.
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		Control ( Control* _Root, const std::string& _Name, const std::string& _Class, const ControlType _Type, const unsigned long Style, const unsigned long StyleEx )
		{
			this->Name = _Name;
			this->Handle = NULL;
			this->Root = _Root;
			this->Type = _Type;
			
			this->IsClosed = false;;
			this->IsClicked = false;
			
			auto RootHandle = HWND(NULL);
			if(this->Root) RootHandle = this->Root->Handle;

			this->Handle = CreateWindowEx
			(
				StyleEx,
				_Class.c_str(),
				"",
				Style,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				RootHandle,
				NULL,
				InstanceHandle,
				NULL
			);
		}

		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Deleted to prevent stupid bugs.
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		Control ( const Control& _Left ) = delete;
		Control ( Control&& _Left ) = delete;

		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Destructor.
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		~Control ( void )
		{
			if(this->Type != ControlType::ROOT)
			{
				for(auto& Child : this->Children) delete Child.second;
				Children.clear();
				if(!this->IsClosed) DestroyWindow(this->Handle);
			}
		}

		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Get child control by name.
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto operator[] ( const std::string& _Name ) -> Control&
		{
			auto Child = this->Children.find(_Name);
			
			if(Child != this->Children.end()) return *Child->second;

			else
			{
				auto Message = std::string("Control: ") + this->Name + std::string(" had no child named: ") + _Name + std::string("!");
				MessageBox(NULL, Message.c_str(), "WUI", MB_ICONERROR | MB_OK);
				return *this;
			}
		}

		// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Create new window with given traits.
		// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto newWindow ( const std::string& _Name, const WindowTraits& _Traits ) -> void
		{
			if(this->Children.count(_Name) == 0)
			{
				if(this->Type == ControlType::ROOT)
				{
					this->Children[_Name] = new Control(nullptr, _Name, "WUI_WINDOW", ControlType::WINDOW, _Traits.buildStyle(), _Traits.buildStyleEx());
					Controls[(*this)[_Name].Handle] = &(*this)[_Name];
				}

				else
				{
					auto TraitsMod = _Traits;
					TraitsMod.IsChild = true;

					this->Children[_Name] = new Control(this, _Name, "WUI_WINDOW", ControlType::WINDOW, TraitsMod.buildStyle(), TraitsMod.buildStyleEx());
					Controls[(*this)[_Name].Handle] = &(*this)[_Name];
				}
			}

			else
			{
				auto Message = std::string("Failed to create window: ") + _Name + std::string("! - Name collision.");
				MessageBox(NULL, Message.c_str(), "WUI", MB_ICONERROR | MB_OK);
			}
		}

		// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Used to create common controls.
		// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto newControl ( const std::string& _Name, const std::string& _Class, const ControlType _Type, const unsigned long _Style ) -> void
		{
			if(this->Children.count(_Name) == 0)
			{
				if(this->Type != ControlType::ROOT)
				{
					this->Children[_Name] = new Control(this, _Name, _Class, _Type, _Style, unsigned long(0));
					Controls[(*this)[_Name].Handle] = &(*this)[_Name];
				}

				else
				{
					auto Message = std::string("Failed to create control: ") + _Name + std::string("! - Controls not allowed for root window.");
					MessageBox(NULL, Message.c_str(), "WUI", MB_ICONERROR | MB_OK);
				}
			}
		
			else
			{
				auto Message = std::string("Failed to create control: ") + _Name + std::string("! - Name collision.");
				MessageBox(NULL, Message.c_str(), "WUI", MB_ICONERROR | MB_OK);
			}
		}

		// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Button family: Push button.
		// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto newButton ( const std::string& _Name, const std::string& _Text = "", const bool _IsVisible = true ) -> Control&
		{
			auto Style = unsigned long(0);

			Style |= WS_CHILD;
			Style |= BS_NOTIFY;
			Style |= BS_PUSHBUTTON;

			this->newControl(_Name, "BUTTON", ControlType::BUTTON, Style);

			(*this)[_Name].setText(_Text);
			if(_IsVisible) (*this)[_Name].show();

			return (*this)[_Name];
		}

		// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Button family: Checkbox.
		// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto newCheckBox ( const std::string& _Name, const std::string& _Text = "", const bool _IsVisible = true ) -> Control&
		{
			auto Style = unsigned long(0);

			Style |= WS_CHILD;
			Style |= BS_CHECKBOX;

			this->newControl(_Name, "BUTTON", ControlType::CHECKBOX, Style);

			(*this)[_Name].setText(_Text);
			if(_IsVisible) (*this)[_Name].show();

			return (*this)[_Name];
		}

		// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Button family: Radio button.
		// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto newRadioButton ( const std::string& _Name, const std::string& _Text = "", const bool _IsVisible = true ) -> Control&
		{
			auto Style = unsigned long(0);

			Style |= WS_CHILD;
			Style |= BS_RADIOBUTTON;

			this->newControl(_Name, "BUTTON", ControlType::RADIOBUTTON, Style);

			(*this)[_Name].setText(_Text);
			if(_IsVisible) (*this)[_Name].show();

			return (*this)[_Name];
		}

		// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Button family: Groupbox.
		// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto newGroupBox ( const std::string& _Name, const std::string& _Text = "", const bool _IsVisible = true ) -> Control&
		{
			auto Style = unsigned long(0);

			Style |= WS_CHILD;
			Style |= BS_GROUPBOX;

			this->newControl(_Name, "BUTTON", ControlType::GROUPBOX, Style);

			(*this)[_Name].setText(_Text);
			if(_IsVisible) (*this)[_Name].show();

			return (*this)[_Name];
		}

		// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Edit family: Output box.
		// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto newOutputBox ( const std::string& _Name, const std::string& _Text = "", const AlignH _Aligment = AlignH::LEFT, const bool _IsVisible = true ) -> Control&
		{
			auto Style = unsigned long(0);

			Style |= WS_CHILD;
			Style |= WS_BORDER;
			Style |= ES_READONLY;
			if(_Aligment == AlignH::LEFT) Style |= ES_LEFT;
			if(_Aligment == AlignH::RIGHT) Style |= ES_RIGHT;
			if(_Aligment == AlignH::CENTER) Style |= ES_CENTER;

			this->newControl(_Name, "EDIT", ControlType::OUTPUTBOX, Style);

			(*this)[_Name].setText(_Text);
			if(_IsVisible) (*this)[_Name].show();

			return (*this)[_Name];
		}

		// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Edit family: Input box.
		// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto newInputBox ( const std::string& _Name, const std::string& _Text = "", const AlignH _Aligment = AlignH::LEFT, const bool _IsVisible = true ) -> Control&
		{
			auto Style = unsigned long(0);

			Style |= WS_CHILD;
			Style |= WS_BORDER;
			if(_Aligment == AlignH::LEFT) Style |= ES_LEFT;
			if(_Aligment == AlignH::RIGHT) Style |= ES_RIGHT;
			if(_Aligment == AlignH::CENTER) Style |= ES_CENTER;

			this->newControl(_Name, "EDIT", ControlType::INPUTBOX, Style);

			(*this)[_Name].setText(_Text);
			if(_IsVisible) (*this)[_Name].show();

			return (*this)[_Name];
		}

		// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Static family: Static text.
		// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto newText ( const std::string& _Name, const std::string& _Text = "", const AlignH _Aligment = AlignH::LEFT, const bool _IsVisible = true ) -> Control&
		{
			auto Style = unsigned long(0);

			Style |= WS_CHILD;
			if(_Aligment == AlignH::LEFT) Style |= SS_LEFT;
			if(_Aligment == AlignH::RIGHT) Style |= SS_RIGHT;
			if(_Aligment == AlignH::CENTER) Style |= SS_CENTER;

			this->newControl(_Name, "STATIC", ControlType::TEXT, Style);

			(*this)[_Name].setText(_Text);
			if(_IsVisible) (*this)[_Name].show();

			return (*this)[_Name];
		}

		// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Static family: Static image.
		// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto newImageBox ( const std::string& _Name, const bool _IsVisible = true ) -> Control&
		{
			auto Style = unsigned long(0);

			Style |= WS_CHILD;
			Style |= SS_BITMAP;
			Style |= SS_REALSIZECONTROL;
			

			this->newControl(_Name, "STATIC", ControlType::IMAGE, Style);

			if(_IsVisible) (*this)[_Name].show();

			return (*this)[_Name];
		}

		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Force control redraw.
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto redraw ( void ) -> void
		{
			RedrawWindow(this->Handle, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW);
			UpdateWindow(this->Handle);
		}
		
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Show control.
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto show ( void ) -> void
		{
			ShowWindow(this->Handle, SW_SHOW);
			UpdateWindow(this->Handle);
		}

		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Hide control.
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto hide ( void ) -> void
		{
			ShowWindow(this->Handle, SW_HIDE);
			UpdateWindow(this->Handle);
		}

		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Minimize window.
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto minimize ( void ) -> void
		{
			ShowWindow(this->Handle, SW_MINIMIZE);
			UpdateWindow(this->Handle);
		}

		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Maximize window.
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto maximize ( void ) -> void
		{
			ShowWindow(this->Handle, SW_MAXIMIZE);
			UpdateWindow(this->Handle);
		}

		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Restore window.
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto restore ( void ) -> void
		{
			ShowWindow(this->Handle, SW_RESTORE);
			UpdateWindow(this->Handle);
		}

		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Set control's position.
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto setPosition ( const int _X, const int _Y ) -> void
		{
			SetWindowPos(this->Handle, NULL, _X, _Y, 0 , 0, SWP_NOSIZE | SWP_NOREPOSITION);
		}

		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Set control's dimensions.
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto setDimensions ( const int _Width, const int _Height ) -> void
		{
			if(this->Type == ControlType::WINDOW)
			{
				auto Rect = RECT{0, 0, _Width, _Height};

				AdjustWindowRectEx
				(
					&Rect,
					(DWORD)GetWindowLong(this->Handle, GWL_STYLE),
					GetMenu(this->Handle) != NULL,
					(DWORD)GetWindowLong(this->Handle, GWL_EXSTYLE)
				);

				SetWindowPos(this->Handle, NULL, 0, 0, Rect.right - Rect.left, Rect.bottom - Rect.top, SWP_NOMOVE | SWP_NOREPOSITION);
			}

			else
			{
				SetWindowPos(this->Handle, NULL, 0, 0, _Width , _Height, SWP_NOMOVE | SWP_NOREPOSITION);
			}
		}

		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Get control's dimensions.
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto getDimensions ( void ) -> std::array<long,2>
		{
			auto Rect = RECT{};
			GetWindowRect(this->Handle, &Rect);
			
			auto Dimensions = std::array<long,2>();
			Dimensions[0] = Rect.right - Rect.left;
			Dimensions[1] = Rect.bottom - Rect.top;

			return Dimensions;
		}

		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Place window in center of the screen.
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto placeAtScreenCenter ( const int _Width, const int _Height ) -> void
		{
			auto PosX = (GetSystemMetrics(SM_CXSCREEN) / 2) - (_Width / 2);
			auto PoxY = (GetSystemMetrics(SM_CYSCREEN) / 2) - (_Height / 2);

			this->setPosition(PosX, PoxY);
			this->setDimensions(_Width, _Height);
		}

		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Set control's text.
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto setText ( std::string _Text ) -> void
		{
			SetWindowText(this->Handle, _Text.c_str());
		}

		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Get control's text.
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto getText ( void ) -> std::string
		{
			auto Lenght = SendMessage(this->Handle, WM_GETTEXTLENGTH, 0, 0);
			auto Buffer = new char[Lenght+1];
			SendMessage(this->Handle, WM_GETTEXT, WPARAM(Lenght+1), LPARAM(Buffer));
			auto Text = std::string(Buffer);
			delete[] Buffer;
			return Text;
		}

		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Set static image's bitmap.
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto setBitmap ( void* _BmpHnd ) -> void
		{
			if(this->Type == ControlType::IMAGE)
			{
				SendMessage(this->Handle, STM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(_BmpHnd));
			}
		}

		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Get checkbox's or radio button's status.
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto isChecked ( void ) -> bool
		{
			if((this->Type == ControlType::CHECKBOX) || (this->Type == ControlType::RADIOBUTTON))
			{
				auto Result = SendMessage(this->Handle, BM_GETSTATE, 0, 0);
				if(Result == BST_CHECKED) return true;
			}

			return false;
		}

		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Generate event: Window closed.
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto close ( const bool _AlreadyDestroyed = false ) -> void
		{
			this->IsClosed = true;
			if(!_AlreadyDestroyed) DestroyWindow(this->Handle);
		}
		
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Event: Window closed.
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto onClose ( void ) -> bool
		{
			if(this->IsClosed) return true;
			return false;
		}

		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Generate event: Button click.
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto click ( void ) -> void
		{
			this->IsClicked = true;
		}

		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Event: Button click.
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto onClick ( void ) -> bool
		{
			if(this->IsClicked)
			{
				this->IsClicked = false;
				return true;
			}
			
			return false;
		}

		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Capture window's client area.
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto captureClientArea ( const std::string& _Bitmap ) -> void
		{
			auto Bitmap = Bitmaps.find(_Bitmap);
			
			if(Bitmap != Bitmaps.end())
			{
				auto WndDims = this->getDimensions();
				auto WndDC = GetDC(this->Handle);
				auto CapDC = CreateCompatibleDC(WndDC);

				SelectObject(CapDC, Bitmap->second.Handle);
				BitBlt(CapDC, 0, 0, WndDims[0], WndDims[1], WndDC, 0, 0, SRCCOPY | CAPTUREBLT);

				DeleteDC(CapDC);
				ReleaseDC(this->Handle, WndDC);
			}

			else
			{
				auto Message = std::string("captureClientArea(): ") + _Bitmap + std::string(" not found!");
				MessageBox(NULL, Message.c_str(), "WUI", MB_ICONERROR | MB_OK);
			}
		}
	};

	// Root window.
	Control RootWnd;

	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Handle all windows messages.
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	auto CALLBACK MsgHandler ( HWND _Handle, UINT _Message, WPARAM ParamW, LPARAM ParamL ) -> LRESULT
	{
		// Window destruction event.
		if(_Message == WM_DESTROY)
		{
			Controls[_Handle]->close(true);
			return 0;
		}

		// Button clicked.
		if(_Message == WM_COMMAND)
		{
			if(HIWORD(ParamW) == BN_CLICKED)
			{
				Controls[HWND(ParamL)]->click();
				return 0;
			}
		}

		// Pass unhandled messages to default handler.
		return DefWindowProc(_Handle, _Message, ParamW, ParamL);
	}
	
	// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Initialize WindowsUI for use. If _InstanceHandle is NULL GetModuleHandle() will be used to get one.
	// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	auto init ( HINSTANCE _InstanceHandle = NULL ) -> void
	{
		if(!IsInit)
		{
			// Get instance handle automatically.
			if(_InstanceHandle == NULL)
			{
				InstanceHandle = GetModuleHandle(NULL);
			}

			// Use provided instance handle.
			else
			{
				InstanceHandle = _InstanceHandle;
			}
		
			// Create default window template.
			WindowClass.cbSize = sizeof(WindowClass);
			WindowClass.style = CS_HREDRAW | CS_VREDRAW;
			WindowClass.lpfnWndProc = MsgHandler;
			WindowClass.cbClsExtra = 0;
			WindowClass.cbWndExtra = 0;
			WindowClass.hInstance = InstanceHandle;
			WindowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
			WindowClass.hbrBackground = HBRUSH(CTLCOLOR_DLG + int(1));
			WindowClass.lpszMenuName = NULL;
			WindowClass.lpszClassName = "WUI_WINDOW";
			WindowClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

			RegisterClassEx(&WindowClass);

			// Load common controls.
			INITCOMMONCONTROLSEX Flags;
			Flags.dwSize = sizeof(Flags);
			Flags.dwICC = ICC_STANDARD_CLASSES;
			InitCommonControlsEx(&Flags);

			// Set as initialised.
			IsInit = true;
		}
	}

	// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Delete all Windows UI resources.
	// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	auto kill ( void ) -> void
	{
		for(auto Bmp : Bitmaps) DeleteObject(Bmp.second.Handle);
		Bitmaps.clear();

		for(auto Child : RootWnd.Children) delete Child.second;
		RootWnd.Children.clear();
	}

	// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Process windows messages.
	// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	auto update ( bool _Block = false ) -> void
	{
		MSG Message;

		if(_Block)
		{
			GetMessage(&Message, NULL, 0, 0);
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		else
		{
			while(PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&Message);
				DispatchMessage(&Message);
			}
		}
	}
}

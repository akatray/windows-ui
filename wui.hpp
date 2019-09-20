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
#include <string>
#include <windows.h>
#include <commctrl.h>

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Windows UI.
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
namespace wui
{
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Globals.
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	WNDCLASSEX WindowClass;
	HINSTANCE InstanceHandle;
	struct Control;
	std::map<HWND, Control*> Controls;

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
		WindowTraits ( void )
		{
			this->IsChild = false;
			this->IsTitled = false;
			this->IsResizeble = false;
			this->IsAlwaysOnTop = false;
			this->HasSysMenu = false;
			this->HasBorder = false;
			this->HasHScrollBar = false;
			this->HasVScrollBar = false;
			this->CanBeMinimized = false;
			this->CanBeMaximized = false;
			this->CanAcceptFiles = false;
		}
		
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Build window style.
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto buildStyle ( void ) const -> unsigned long 
		{
			auto Style = unsigned long(0);

			if(this->IsChild == true) Style |= WS_CHILD;
			if(this->IsTitled == true) Style |= WS_CAPTION;
			if(this->IsResizeble == true) Style |= WS_SIZEBOX;
			if(this->HasSysMenu == true) Style |= WS_SYSMENU;
			if(this->HasBorder == true) Style |= WS_BORDER;
			if(this->HasHScrollBar == true) Style |= WS_HSCROLL;
			if(this->HasVScrollBar == true) Style |= WS_VSCROLL;
			if(this->CanBeMinimized == true) Style |= WS_MINIMIZEBOX;
			if(this->CanBeMaximized == true) Style |= WS_MAXIMIZEBOX;

			return Style;
		}

		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Build extended window style.
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto buildStyleEx ( void ) const -> unsigned long
		{
			auto Style = unsigned long(0);

			if(this->IsAlwaysOnTop == true) Style |= WS_EX_TOPMOST;
			if(this->IsToolWindow == true) Style |= WS_EX_TOOLWINDOW;
			if(this->CanAcceptFiles == true) Style |= WS_EX_ACCEPTFILES;

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
		auto newButton ( const std::string& _Name, const std::string _Text = "", const bool _IsVisible = true ) -> Control&
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
		auto newCheckBox ( const std::string& _Name, const std::string _Text = "", const bool _IsVisible = true ) -> Control&
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
		auto newRadioButton ( const std::string& _Name, const std::string _Text = "", const bool _IsVisible = true ) -> Control&
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
		auto newGroupBox ( const std::string& _Name, const std::string _Text = "", const bool _IsVisible = true ) -> Control&
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
		auto newOutputBox ( const std::string& _Name, const std::string _Text = "", const AlignH _Aligment = AlignH::LEFT, const bool _IsVisible = true ) -> Control&
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
		auto newInputBox ( const std::string& _Name, const std::string _Text = "", const AlignH _Aligment = AlignH::LEFT, const bool _IsVisible = true ) -> Control&
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
		auto newText ( const std::string& _Name, const std::string _Text = "", const AlignH _Aligment = AlignH::LEFT, const bool _IsVisible = true ) -> Control&
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
		auto setPosition ( int _X, int _Y ) -> void
		{
			SetWindowPos(this->Handle, NULL, _X, _Y, 0 , 0, SWP_NOSIZE | SWP_NOREPOSITION);
		}

		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Set control's dimensions.
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto setDimensions ( int _Width, int _Height ) -> void
		{
			SetWindowPos(this->Handle, NULL, 0, 0, _Width , _Height, SWP_NOMOVE | SWP_NOREPOSITION);
		}

		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Place window in center of the screen.
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto placeAtScreenCenter ( int _Width, int _Height ) -> void
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
		auto close ( bool _AlreadyDestroyed = false ) -> void
		{
			this->IsClosed = true;
			if(!_AlreadyDestroyed) DestroyWindow(this->Handle);
		}
		
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Event: Window closed.
		// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		auto onClose ( void ) -> bool
		{
			if(this->IsClosed)
			{
				return true;
			}

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
	};

	// Root window.
	Control RootWnd;

	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Bitmaps management.
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
	auto createBitmap ( std::string _Name, int _Width, int _Height ) -> void
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
	auto updateBitmap ( std::string _Name, void* _Data ) -> void
	{
		if(Bitmaps.count(_Name) != 0)
		{
			std::memcpy(Bitmaps[_Name].Data, _Data, Bitmaps[_Name].Width * Bitmaps[_Name].Height * Bitmaps[_Name].Depth);
		}

		else
		{
			auto Message = std::string("Failed to update bitmap: ") + _Name + std::string("! - Name not found.");
			MessageBox(NULL, Message.c_str(), "WUI", MB_ICONERROR | MB_OK);
		}
	}

	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Get bitmap's handle by name.
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	auto getBitmap ( std::string _Name) -> void*
	{
		if(Bitmaps.count(_Name) != 0)
		{
			return Bitmaps[_Name].Handle;
		}

		else
		{
			auto Message = std::string("Failed to get bitmap: ") + _Name + std::string("! - Name not found.");
			MessageBox(NULL, Message.c_str(), "WUI", MB_ICONERROR | MB_OK);

			return NULL;
		}
	}

	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Delete bitmap.
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	auto killBitmap ( std::string _Name) -> void
	{
		if(Bitmaps.count(_Name) != 0)
		{
			DeleteObject(Bitmaps[_Name].Handle);
			Bitmaps.erase(_Name);
		}

		else
		{
			auto Message = std::string("Failed to kill bitmap: ") + _Name + std::string("! - Name not found.");
			MessageBox(NULL, Message.c_str(), "WUI", MB_ICONERROR | MB_OK);
		}
	}

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

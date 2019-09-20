// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Imports.
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "./../wui.hpp"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Official WindowsUI Calculator.
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
auto WINAPI WinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow ) -> int
{
	// Setup WindowsUI for use.
	wui::init(hInstance); // If hInstance is not given, GetModuleHandle(NULL) will be used instead. Might be a problem if called from dll.

	// Setup main window.
	wui::RootWnd.newWindow("CalcWnd", wui::getWindowTraitsFixed()); // Window is created from root window. Root window's only purpose is to create other windows.
	
	auto& CalcWnd = wui::RootWnd["CalcWnd"]; // Operator[] returns reference to child control of given name.
	CalcWnd.placeAtScreenCenter(227, 300); // Set window size and place it at desktop's center.
	CalcWnd.setText("Calculatron 3000"); // Sets title bar for window, name for buttons, content for input/output boxes, etc.
	CalcWnd.show(); // Window control are hidden by default.

	// Setup digits panel.
	CalcWnd.newWindow("OutputPanel", wui::getWindowTraitsPanel());
	CalcWnd["OutputPanel"].setPosition(5, 5);
	CalcWnd["OutputPanel"].setDimensions(202, 57);
	CalcWnd["OutputPanel"].show();

	CalcWnd["OutputPanel"].newOutputBox("Pending", "0"); // Create outputbox control. Visible by default.
	CalcWnd["OutputPanel"]["Pending"].setDimensions(190, 20);
	CalcWnd["OutputPanel"]["Pending"].setPosition(5, 5);

	CalcWnd["OutputPanel"].newOutputBox("CurrValue", "0");
	CalcWnd["OutputPanel"]["CurrValue"].setDimensions(190, 20);
	CalcWnd["OutputPanel"]["CurrValue"].setPosition(5, 30);
	
	// Setup digits panel.
	CalcWnd.newWindow("DigitsPanel", wui::getWindowTraitsPanel());
	CalcWnd["DigitsPanel"].setPosition(5, 67);
	CalcWnd["DigitsPanel"].setDimensions(118, 155);
	CalcWnd["DigitsPanel"].show();

	CalcWnd["DigitsPanel"].newButton("NumKey_7", "7"); 
	CalcWnd["DigitsPanel"]["NumKey_7"].setDimensions(32, 32);
	CalcWnd["DigitsPanel"]["NumKey_7"].setPosition(5, 5);

	CalcWnd["DigitsPanel"].newButton("NumKey_8", "8");
	CalcWnd["DigitsPanel"]["NumKey_8"].setDimensions(32, 32);
	CalcWnd["DigitsPanel"]["NumKey_8"].setPosition(42, 5);

	CalcWnd["DigitsPanel"].newButton("NumKey_9", "9");
	CalcWnd["DigitsPanel"]["NumKey_9"].setDimensions(32, 32);
	CalcWnd["DigitsPanel"]["NumKey_9"].setPosition(79, 5);

	CalcWnd["DigitsPanel"].newButton("NumKey_4", "4");
	CalcWnd["DigitsPanel"]["NumKey_4"].setDimensions(32, 32);
	CalcWnd["DigitsPanel"]["NumKey_4"].setPosition(5, 42);

	CalcWnd["DigitsPanel"].newButton("NumKey_5", "5");
	CalcWnd["DigitsPanel"]["NumKey_5"].setDimensions(32, 32);
	CalcWnd["DigitsPanel"]["NumKey_5"].setPosition(42, 42);

	CalcWnd["DigitsPanel"].newButton("NumKey_6", "6");
	CalcWnd["DigitsPanel"]["NumKey_6"].setDimensions(32, 32);
	CalcWnd["DigitsPanel"]["NumKey_6"].setPosition(79, 42);

	CalcWnd["DigitsPanel"].newButton("NumKey_1", "1");
	CalcWnd["DigitsPanel"]["NumKey_1"].setDimensions(32, 32);
	CalcWnd["DigitsPanel"]["NumKey_1"].setPosition(5, 79);

	CalcWnd["DigitsPanel"].newButton("NumKey_2", "2");
	CalcWnd["DigitsPanel"]["NumKey_2"].setDimensions(32, 32);
	CalcWnd["DigitsPanel"]["NumKey_2"].setPosition(42, 79);

	CalcWnd["DigitsPanel"].newButton("NumKey_3", "3");
	CalcWnd["DigitsPanel"]["NumKey_3"].setDimensions(32, 32);
	CalcWnd["DigitsPanel"]["NumKey_3"].setPosition(79, 79);

	CalcWnd["DigitsPanel"].newButton("OpKeyNeg", "+/-");
	CalcWnd["DigitsPanel"]["OpKeyNeg"].setDimensions(32, 32);
	CalcWnd["DigitsPanel"]["OpKeyNeg"].setPosition(5, 116);

	CalcWnd["DigitsPanel"].newButton("NumKey_0", "0");
	CalcWnd["DigitsPanel"]["NumKey_0"].setDimensions(32, 32);
	CalcWnd["DigitsPanel"]["NumKey_0"].setPosition(42, 116);

	CalcWnd["DigitsPanel"].newButton("OpKeyPoint", ".");
	CalcWnd["DigitsPanel"]["OpKeyPoint"].setDimensions(32, 32);
	CalcWnd["DigitsPanel"]["OpKeyPoint"].setPosition(79, 116);

	// Setup ops panel.
	CalcWnd.newWindow("OpsPanel", wui::getWindowTraitsPanel());
	CalcWnd["OpsPanel"].setPosition(128, 67);
	CalcWnd["OpsPanel"].setDimensions(79, 155);
	CalcWnd["OpsPanel"].show();

	CalcWnd["OpsPanel"].newButton("OpAdd", "+");
	CalcWnd["OpsPanel"]["OpAdd"].setDimensions(32, 32);
	CalcWnd["OpsPanel"]["OpAdd"].setPosition(5, 5);

	CalcWnd["OpsPanel"].newButton("OpSub", "-");
	CalcWnd["OpsPanel"]["OpSub"].setDimensions(32, 32);
	CalcWnd["OpsPanel"]["OpSub"].setPosition(5, 42);

	CalcWnd["OpsPanel"].newButton("OpMul", "*");
	CalcWnd["OpsPanel"]["OpMul"].setDimensions(32, 32);
	CalcWnd["OpsPanel"]["OpMul"].setPosition(5, 79);

	CalcWnd["OpsPanel"].newButton("OpDiv", "/");
	CalcWnd["OpsPanel"]["OpDiv"].setDimensions(32, 32);
	CalcWnd["OpsPanel"]["OpDiv"].setPosition(5, 116);

	CalcWnd["OpsPanel"].newButton("OpC", "C");
	CalcWnd["OpsPanel"]["OpC"].setDimensions(32, 32);
	CalcWnd["OpsPanel"]["OpC"].setPosition(42, 5);

	CalcWnd["OpsPanel"].newButton("OpCE", "CE");
	CalcWnd["OpsPanel"]["OpCE"].setDimensions(32, 32);
	CalcWnd["OpsPanel"]["OpCE"].setPosition(42, 42);

	CalcWnd["OpsPanel"].newButton("OpEnter", "=");
	CalcWnd["OpsPanel"]["OpEnter"].setDimensions(32, 69);
	CalcWnd["OpsPanel"]["OpEnter"].setPosition(42, 79);

	// Copyright.
	CalcWnd.newText("Copyleft", "(C) 2019 MicroSh*t", wui::AlignH::CENTER);
	CalcWnd["Copyleft"].setDimensions(202, 20);
	CalcWnd["Copyleft"].setPosition(5, 230);

	// Calculator state.
	auto IsPeriodUsed = false;
	auto IsNegativeUsed = false;
	
	auto PendingLValue = double(0.0f);
	auto PendingRValue = double(0.0f);
	auto PendingOp = int(0);
	auto LastOp = int(0);

	// Calculator loop.
	while(!CalcWnd.onClose()) // Will return when window is closed/destroyed.
	{
		// Reset current lvalue.
		if(CalcWnd["OpsPanel"]["OpCE"].onClick()) // Return true once when clicked.
		{
			CalcWnd["OutputPanel"]["CurrValue"].setText("0");
			IsPeriodUsed = false;
			IsNegativeUsed = false;
		}

		// Reset calculator.
		if(CalcWnd["OpsPanel"]["OpC"].onClick())
		{
			CalcWnd["OutputPanel"]["Pending"].setText("0");
			CalcWnd["OutputPanel"]["CurrValue"].setText("0");
			IsPeriodUsed = false;
			IsNegativeUsed = false;
			PendingLValue = 0.0;
			PendingRValue = 0.0;
			PendingOp = 0;
			LastOp = 0;
		}

		// Number inputs.
		if(CalcWnd["DigitsPanel"]["NumKey_1"].onClick()) CalcWnd["OutputPanel"]["CurrValue"].setText(CalcWnd["OutputPanel"]["CurrValue"].getText() + std::to_string(1));
		if(CalcWnd["DigitsPanel"]["NumKey_2"].onClick()) CalcWnd["OutputPanel"]["CurrValue"].setText(CalcWnd["OutputPanel"]["CurrValue"].getText() + std::to_string(2));
		if(CalcWnd["DigitsPanel"]["NumKey_3"].onClick()) CalcWnd["OutputPanel"]["CurrValue"].setText(CalcWnd["OutputPanel"]["CurrValue"].getText() + std::to_string(3));
		if(CalcWnd["DigitsPanel"]["NumKey_4"].onClick()) CalcWnd["OutputPanel"]["CurrValue"].setText(CalcWnd["OutputPanel"]["CurrValue"].getText() + std::to_string(4));
		if(CalcWnd["DigitsPanel"]["NumKey_5"].onClick()) CalcWnd["OutputPanel"]["CurrValue"].setText(CalcWnd["OutputPanel"]["CurrValue"].getText() + std::to_string(5));
		if(CalcWnd["DigitsPanel"]["NumKey_6"].onClick()) CalcWnd["OutputPanel"]["CurrValue"].setText(CalcWnd["OutputPanel"]["CurrValue"].getText() + std::to_string(6));
		if(CalcWnd["DigitsPanel"]["NumKey_7"].onClick()) CalcWnd["OutputPanel"]["CurrValue"].setText(CalcWnd["OutputPanel"]["CurrValue"].getText() + std::to_string(7));
		if(CalcWnd["DigitsPanel"]["NumKey_8"].onClick()) CalcWnd["OutputPanel"]["CurrValue"].setText(CalcWnd["OutputPanel"]["CurrValue"].getText() + std::to_string(8));
		if(CalcWnd["DigitsPanel"]["NumKey_9"].onClick()) CalcWnd["OutputPanel"]["CurrValue"].setText(CalcWnd["OutputPanel"]["CurrValue"].getText() + std::to_string(9));
		if(CalcWnd["DigitsPanel"]["NumKey_0"].onClick()) CalcWnd["OutputPanel"]["CurrValue"].setText(CalcWnd["OutputPanel"]["CurrValue"].getText() + std::to_string(0));

		// Period input.
		if((CalcWnd["DigitsPanel"]["OpKeyPoint"].onClick()) && (IsPeriodUsed == false))
		{
			CalcWnd["OutputPanel"]["CurrValue"].setText(CalcWnd["OutputPanel"]["CurrValue"].getText() + std::string("."));
			IsPeriodUsed = true;
		}

		// Negation input.
		if(CalcWnd["DigitsPanel"]["OpKeyNeg"].onClick()) // OnClick return true only once per click. For deeper logic check for click first.
		{
			if(IsNegativeUsed)
			{
				auto CurrentValue = CalcWnd["OutputPanel"]["CurrValue"].getText();
				CalcWnd["OutputPanel"]["CurrValue"].setText(std::string(&CurrentValue[1])); // Remove first character.
				IsNegativeUsed = false;
			}

			else
			{
				auto CurrentValue = CalcWnd["OutputPanel"]["CurrValue"].getText();
				
				if(CurrentValue[0] != '0' || CurrentValue[1] == '.')
				{
					CalcWnd["OutputPanel"]["CurrValue"].setText(std::string("-") + CurrentValue);
					IsNegativeUsed = true;
				}
			}
		}

		// Fix case where first digit is zero.
		auto CurrentValue = CalcWnd["OutputPanel"]["CurrValue"].getText();
		if(CurrentValue.size() > 1) if(CurrentValue[0] == '0' && CurrentValue[1] != '.') CalcWnd["OutputPanel"]["CurrValue"].setText(std::string(&CurrentValue[1]));

		// Process operation.
		if(CalcWnd["OpsPanel"]["OpEnter"].onClick())
		{
			// Repeat last operation.
			if(PendingOp == 0)
			{
				PendingOp = LastOp;
				CalcWnd["OutputPanel"]["CurrValue"].setText(std::to_string(PendingRValue));
			}
			
			// Add operation.
			if(PendingOp == 1)
			{
				PendingRValue = std::atof(CalcWnd["OutputPanel"]["CurrValue"].getText().c_str());
				PendingLValue = PendingLValue + PendingRValue;
				CalcWnd["OutputPanel"]["Pending"].setText(std::to_string(PendingLValue));
				CalcWnd["OutputPanel"]["CurrValue"].setText("0");
				LastOp = 1;
				PendingOp = 0;
			}

			// Sub operation.
			if(PendingOp == 2)
			{
				PendingRValue = std::atof(CalcWnd["OutputPanel"]["CurrValue"].getText().c_str());
				PendingLValue = PendingLValue - PendingRValue;
				CalcWnd["OutputPanel"]["Pending"].setText(std::to_string(PendingLValue));
				CalcWnd["OutputPanel"]["CurrValue"].setText("0");
				LastOp = 2;
				PendingOp = 0;
			}

			// Mul operation.
			if(PendingOp == 3)
			{
				PendingRValue = std::atof(CalcWnd["OutputPanel"]["CurrValue"].getText().c_str());
				PendingLValue = PendingLValue * PendingRValue;
				CalcWnd["OutputPanel"]["Pending"].setText(std::to_string(PendingLValue));
				CalcWnd["OutputPanel"]["CurrValue"].setText("0");
				LastOp = 3;
				PendingOp = 0;
			}

			// Div operation.
			if(PendingOp == 4)
			{
				PendingRValue = std::atof(CalcWnd["OutputPanel"]["CurrValue"].getText().c_str());
				PendingLValue = PendingLValue / PendingRValue;
				CalcWnd["OutputPanel"]["Pending"].setText(std::to_string(PendingLValue));
				CalcWnd["OutputPanel"]["CurrValue"].setText("0");
				LastOp = 4;
				PendingOp = 0;
			}
		}

		// Wait for operation events.
		auto OperationEvent = int(0);

		if(CalcWnd["OpsPanel"]["OpAdd"].onClick()) OperationEvent = 1;
		if(CalcWnd["OpsPanel"]["OpSub"].onClick()) OperationEvent = 2;
		if(CalcWnd["OpsPanel"]["OpMul"].onClick()) OperationEvent = 3;
		if(CalcWnd["OpsPanel"]["OpDiv"].onClick()) OperationEvent = 4;

		// On operation event.
		if(OperationEvent > 0)
		{
			// Setup operation.
			if(PendingOp == 0)
			{
				PendingLValue = std::atof(CalcWnd["OutputPanel"]["CurrValue"].getText().c_str());
				PendingOp = OperationEvent;
				CalcWnd["OutputPanel"]["CurrValue"].setText("0");

				if(OperationEvent == 1) CalcWnd["OutputPanel"]["Pending"].setText(std::to_string(PendingLValue) + std::string(" + ?"));
				if(OperationEvent == 2) CalcWnd["OutputPanel"]["Pending"].setText(std::to_string(PendingLValue) + std::string(" - ?"));
				if(OperationEvent == 3) CalcWnd["OutputPanel"]["Pending"].setText(std::to_string(PendingLValue) + std::string(" * ?"));
				if(OperationEvent == 4) CalcWnd["OutputPanel"]["Pending"].setText(std::to_string(PendingLValue) + std::string(" / ?"));
			}

			// Change operation.
			else
			{
				PendingOp = 1;

				if(OperationEvent == 1) CalcWnd["OutputPanel"]["Pending"].setText(std::to_string(PendingLValue) + std::string(" + ?"));
				if(OperationEvent == 2) CalcWnd["OutputPanel"]["Pending"].setText(std::to_string(PendingLValue) + std::string(" - ?"));
				if(OperationEvent == 3) CalcWnd["OutputPanel"]["Pending"].setText(std::to_string(PendingLValue) + std::string(" * ?"));
				if(OperationEvent == 4) CalcWnd["OutputPanel"]["Pending"].setText(std::to_string(PendingLValue) + std::string(" / ?"));
			}
		}

		// Process events.
		wui::update(); // Update ui, process events.
	}

	// Delete resources yourself or let OS to clean it for you.
	wui::kill();

	// End.
	return 0;
}

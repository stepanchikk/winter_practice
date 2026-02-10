#pragma once

namespace Lab5 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
		}

	protected:
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::Button^ btnHermite;
	private: System::Windows::Forms::Button^ btnCloud;
	private: System::Windows::Forms::Panel^ drawingPanel;
	private: System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		   void InitializeComponent(void)
		   {
			   this->btnHermite = (gcnew System::Windows::Forms::Button());
			   this->btnCloud = (gcnew System::Windows::Forms::Button());
			   this->drawingPanel = (gcnew System::Windows::Forms::Panel());
			   this->SuspendLayout();
			   // 
			   // btnHermite
			   // 
			   this->btnHermite->Location = System::Drawing::Point(12, 12);
			   this->btnHermite->Name = L"btnHermite";
			   this->btnHermite->Size = System::Drawing::Size(150, 40);
			   this->btnHermite->TabIndex = 0;
			   this->btnHermite->Text = L"Побудувати Ерміта";
			   this->btnHermite->Click += gcnew System::EventHandler(this, &MyForm::btnHermite_Click);
			   // 
			   // btnCloud
			   // 
			   this->btnCloud->Location = System::Drawing::Point(170, 12);
			   this->btnCloud->Name = L"btnCloud";
			   this->btnCloud->Size = System::Drawing::Size(150, 40);
			   this->btnCloud->TabIndex = 1;
			   this->btnCloud->Text = L"Хмарне небо";
			   this->btnCloud->Click += gcnew System::EventHandler(this, &MyForm::btnCloud_Click);
			   // 
			   // drawingPanel
			   // 
			   this->drawingPanel->BackColor = System::Drawing::Color::White;
			   this->drawingPanel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			   this->drawingPanel->Location = System::Drawing::Point(12, 60);
			   this->drawingPanel->Name = L"drawingPanel";
			   this->drawingPanel->Size = System::Drawing::Size(760, 480);
			   this->drawingPanel->TabIndex = 2;
			   // 
			   // MyForm
			   // 
			   this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			   this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			   this->ClientSize = System::Drawing::Size(784, 561);
			   this->Controls->Add(this->drawingPanel);
			   this->Controls->Add(this->btnCloud);
			   this->Controls->Add(this->btnHermite);
			   this->Name = L"MyForm";
			   this->Text = L"Лабораторна 5 - Варіант 23";
			   this->ResumeLayout(false);
		   }
#pragma endregion

		   // Рекурсія для фракталу "Дракон" (Задача 2.7) 
		   void DrawDragonCloud(Graphics^ g, float x1, float y1, float x2, float y2, int k, Random^ rnd)
		   {
			   if (k == 0) {
				   int blue = rnd->Next(180, 255); // Відтінки синього 
				   int green = rnd->Next(100, 200);
				   Pen^ p = gcnew Pen(Color::FromArgb(130, 50, green, blue), 1);
				   g->DrawLine(p, x1, y1, x2, y2);
			   }
			   else {
				   float xn = (x1 + x2) / 2 + (y2 - y1) / 2;
				   float yn = (y1 + y2) / 2 - (x2 - x1) / 2;
				   DrawDragonCloud(g, x1, y1, xn, yn, k - 1, rnd);
				   DrawDragonCloud(g, x2, y2, xn, yn, k - 1, rnd);
			   }
		   }

		   // Обробник для кривої Ерміта (Задача 1.1) 
	private: System::Void btnHermite_Click(System::Object^ sender, System::EventArgs^ e) {
		Graphics^ g = drawingPanel->CreateGraphics();
		g->Clear(Color::White);
		Pen^ pen = gcnew Pen(Color::Blue, 2);

		// Початкові точки та вектори 
		float x1 = 100, y1 = 300, x2 = 500, y2 = 300;
		float vx1 = 200, vy1 = -400, vx2 = 200, vy2 = 400;
		float oldX = x1, oldY = y1;

		for (float t = 0; t <= 1.0; t += 0.01) {
			float t2 = t * t, t3 = t2 * t;
			// Базові поліноми Ерміта 
			float h00 = 2 * t3 - 3 * t2 + 1;
			float h01 = -2 * t3 + 3 * t2;
			float h10 = t3 - 2 * t2 + t;
			float h11 = t3 - t2;

			float currX = h00 * x1 + h10 * vx1 + h01 * x2 + h11 * vx2;
			float currY = h00 * y1 + h10 * vy1 + h01 * y2 + h11 * vy2;
			g->DrawLine(pen, oldX, oldY, currX, currY);
			oldX = currX; oldY = currY;
		}
	}

		   // Обробник для Хмарного неба (Задача 2.7) 
	private: System::Void btnCloud_Click(System::Object^ sender, System::EventArgs^ e) {
		Graphics^ g = drawingPanel->CreateGraphics();
		g->Clear(Color::AliceBlue);
		Random^ rnd = gcnew Random();

		// Малюємо 7 хмар з безпечним рівнем рекурсії K=10 
		for (int i = 0; i < 7; i++) {
			float x = (float)rnd->Next(50, 450);
			float y = (float)rnd->Next(50, 350);
			float len = (float)rnd->Next(100, 200);
			DrawDragonCloud(g, x, y, x + len, y, 10, rnd);
			Application::DoEvents(); // Щоб програма не зависала
		}
	}

	}; 
}
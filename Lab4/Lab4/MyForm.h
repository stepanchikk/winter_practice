#pragma once

using namespace MySql::Data::MySqlClient;
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

namespace Lab4 {

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

		// Оголошуємо елементи
	private: System::Windows::Forms::DataGridView^ dataGridView1;
	private: System::Windows::Forms::TextBox^ txtType;
	private: System::Windows::Forms::TextBox^ txtBrand;
	private: System::Windows::Forms::TextBox^ txtManuf;
	private: System::Windows::Forms::TextBox^ txtSupp;
	private: System::Windows::Forms::DateTimePicker^ dtpDate;
	private: System::Windows::Forms::NumericUpDown^ numPrice;
	private: System::Windows::Forms::Button^ btnAdd;
	private: System::Windows::Forms::Button^ btnReload;
	private: System::Windows::Forms::Label^ lblType;
	private: System::Windows::Forms::Label^ lblBrand;
	private: System::Windows::Forms::Label^ lblManuf;
	private: System::Windows::Forms::Label^ lblSupp;
	private: System::Windows::Forms::Label^ lblDate;
	private: System::Windows::Forms::Label^ lblPrice;

	private: System::ComponentModel::Container^ components;

	private:
		// --- ВАШІ НАЛАШТУВАННЯ БД ---
		// Впишіть свій пароль тут!
		String^ connString = "Server=localhost;Database=alko;Uid=root;Pwd=1234;";

		// Завантаження даних
		void LoadData() {
			MySqlConnection^ conn = gcnew MySqlConnection(connString);
			try {
				conn->Open();
				MySqlDataAdapter^ da = gcnew MySqlDataAdapter("SELECT * FROM alkos", conn);
				DataTable^ dt = gcnew DataTable();
				da->Fill(dt);
				dataGridView1->DataSource = dt;
			}
			catch (Exception^ ex) {
				MessageBox::Show("Помилка БД: " + ex->Message);
			}
			finally {
				conn->Close();
			}
		}

		// Клік по кнопці "Оновити"
		System::Void btnReload_Click(System::Object^ sender, System::EventArgs^ e) {
			LoadData();
		}

		// Клік по кнопці "Додати"
		System::Void btnAdd_Click(System::Object^ sender, System::EventArgs^ e) {
			String^ type = txtType->Text;
			String^ brand = txtBrand->Text;
			String^ manuf = txtManuf->Text;
			String^ supp = txtSupp->Text;
			String^ date = dtpDate->Value.ToString("yyyy-MM-dd");
			Decimal price = numPrice->Value;

			if (type == "" || brand == "") {
				MessageBox::Show("Заповніть Вид та Марку!");
				return;
			}

			MySqlConnection^ conn = gcnew MySqlConnection(connString);
			try {
				conn->Open();
				String^ sql = "INSERT INTO alkos (Type, Brand, Manufacturer, Supplier, ExpirationDate, Price) VALUES (@t, @b, @m, @s, @d, @p)";
				MySqlCommand^ cmd = gcnew MySqlCommand(sql, conn);

				cmd->Parameters->AddWithValue("@t", type);
				cmd->Parameters->AddWithValue("@b", brand);
				cmd->Parameters->AddWithValue("@m", manuf);
				cmd->Parameters->AddWithValue("@s", supp);
				cmd->Parameters->AddWithValue("@d", date);
				cmd->Parameters->AddWithValue("@p", price);

				cmd->ExecuteNonQuery();
				MessageBox::Show("Додано!");
				txtType->Text = ""; txtBrand->Text = "";
				LoadData();
			}
			catch (Exception^ ex) {
				MessageBox::Show("Помилка: " + ex->Message);
			}
			finally {
				conn->Close();
			}
		}

		// При запуску
		System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
			LoadData();
		}

		// --- НАЛАШТУВАННЯ ДИЗАЙНУ ---
		void InitializeComponent(void)
		{
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->txtType = (gcnew System::Windows::Forms::TextBox());
			this->txtBrand = (gcnew System::Windows::Forms::TextBox());
			this->txtManuf = (gcnew System::Windows::Forms::TextBox());
			this->txtSupp = (gcnew System::Windows::Forms::TextBox());
			this->dtpDate = (gcnew System::Windows::Forms::DateTimePicker());
			this->numPrice = (gcnew System::Windows::Forms::NumericUpDown());
			this->btnAdd = (gcnew System::Windows::Forms::Button());
			this->btnReload = (gcnew System::Windows::Forms::Button());
			this->lblType = (gcnew System::Windows::Forms::Label());
			this->lblBrand = (gcnew System::Windows::Forms::Label());
			this->lblManuf = (gcnew System::Windows::Forms::Label());
			this->lblSupp = (gcnew System::Windows::Forms::Label());
			this->lblDate = (gcnew System::Windows::Forms::Label());
			this->lblPrice = (gcnew System::Windows::Forms::Label());

			// ВИПРАВЛЕНО ТУТ: Додано символ ^ після ISupportInitialize
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numPrice))->BeginInit();
			this->SuspendLayout();

			// 
			// dataGridView1
			// 
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->Location = System::Drawing::Point(300, 12);
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->Size = System::Drawing::Size(480, 400);
			this->dataGridView1->TabIndex = 0;

			// 
			// Поля та Лейбли
			// 
			this->lblType->Text = L"Вид:";
			this->lblType->Location = System::Drawing::Point(12, 15);
			this->txtType->Location = System::Drawing::Point(100, 12);
			this->txtType->Size = System::Drawing::Size(180, 20);

			this->lblBrand->Text = L"Марка:";
			this->lblBrand->Location = System::Drawing::Point(12, 45);
			this->txtBrand->Location = System::Drawing::Point(100, 42);
			this->txtBrand->Size = System::Drawing::Size(180, 20);

			this->lblManuf->Text = L"Виробник:";
			this->lblManuf->Location = System::Drawing::Point(12, 75);
			this->txtManuf->Location = System::Drawing::Point(100, 72);
			this->txtManuf->Size = System::Drawing::Size(180, 20);

			this->lblSupp->Text = L"Постачал.:";
			this->lblSupp->Location = System::Drawing::Point(12, 105);
			this->txtSupp->Location = System::Drawing::Point(100, 102);
			this->txtSupp->Size = System::Drawing::Size(180, 20);

			this->lblDate->Text = L"Придатний до:";
			this->lblDate->Location = System::Drawing::Point(12, 135);
			this->dtpDate->Location = System::Drawing::Point(100, 132);
			this->dtpDate->Size = System::Drawing::Size(180, 20);

			this->lblPrice->Text = L"Ціна:";
			this->lblPrice->Location = System::Drawing::Point(12, 165);
			this->numPrice->Location = System::Drawing::Point(100, 162);
			this->numPrice->Size = System::Drawing::Size(180, 20);
			this->numPrice->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, 0 });

			// 
			// Кнопки
			// 
			this->btnAdd->Text = L"Додати запис";
			this->btnAdd->Location = System::Drawing::Point(12, 200);
			this->btnAdd->Size = System::Drawing::Size(270, 40);
			this->btnAdd->UseVisualStyleBackColor = true;
			this->btnAdd->Click += gcnew System::EventHandler(this, &MyForm::btnAdd_Click);

			this->btnReload->Text = L"Оновити таблицю";
			this->btnReload->Location = System::Drawing::Point(12, 250);
			this->btnReload->Size = System::Drawing::Size(270, 40);
			this->btnReload->UseVisualStyleBackColor = true;
			this->btnReload->Click += gcnew System::EventHandler(this, &MyForm::btnReload_Click);

			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(800, 450);
			this->Controls->Add(this->btnReload);
			this->Controls->Add(this->btnAdd);
			this->Controls->Add(this->numPrice);
			this->Controls->Add(this->dtpDate);
			this->Controls->Add(this->txtSupp);
			this->Controls->Add(this->txtManuf);
			this->Controls->Add(this->txtBrand);
			this->Controls->Add(this->txtType);
			this->Controls->Add(this->dataGridView1);
			this->Controls->Add(this->lblType);
			this->Controls->Add(this->lblBrand);
			this->Controls->Add(this->lblManuf);
			this->Controls->Add(this->lblSupp);
			this->Controls->Add(this->lblDate);
			this->Controls->Add(this->lblPrice);
			this->Name = L"MyForm";
			this->Text = L"Лабораторна 4 - Облік Напоїв";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);

			// І ТУТ ТАКОЖ ВИПРАВЛЕНО (Додано ^)
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numPrice))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();
		}
	};
}
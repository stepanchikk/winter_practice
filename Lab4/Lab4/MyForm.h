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
			if (components) delete components;
		}

	private: System::Windows::Forms::DataGridView^ dataGridView1;
	private: System::Windows::Forms::TextBox^ txtType;
	private: System::Windows::Forms::TextBox^ txtBrand;
	private: System::Windows::Forms::TextBox^ txtManuf;
	private: System::Windows::Forms::TextBox^ txtSupp;
	private: System::Windows::Forms::DateTimePicker^ dtpDate;
	private: System::Windows::Forms::NumericUpDown^ numPrice;

	private: System::Windows::Forms::Button^ btnAdd;
	private: System::Windows::Forms::Button^ btnReload;
	private: System::Windows::Forms::Button^ btnEdit;
	private: System::Windows::Forms::Button^ btnSearch;
	private: System::Windows::Forms::TextBox^ txtSearch;

	private: System::Windows::Forms::Label^ lblType;
	private: System::Windows::Forms::Label^ lblBrand;
	private: System::Windows::Forms::Label^ lblManuf;
	private: System::Windows::Forms::Label^ lblSupp;
	private: System::Windows::Forms::Label^ lblDate;
	private: System::Windows::Forms::Label^ lblPrice;
	private: System::Windows::Forms::Label^ lblSearch;

	private: System::ComponentModel::Container^ components;
	private: int currentId = 0;

	private:
		String^ connString = "Server=localhost;Database=alko;Uid=root;Pwd=1234;";

		void LoadData(String^ filter) {
			MySqlConnection^ conn = gcnew MySqlConnection(connString);
			try {
				conn->Open();
				String^ query = "SELECT * FROM alkos";
				if (filter != "") {
					query += " WHERE Type LIKE '%" + filter + "%' OR Brand LIKE '%" + filter + "%'";
				}
				MySqlDataAdapter^ da = gcnew MySqlDataAdapter(query, conn);
				DataTable^ dt = gcnew DataTable();
				da->Fill(dt);
				dataGridView1->DataSource = dt;
			}
			catch (Exception^ ex) {
				MessageBox::Show("Помилка: " + ex->Message);
			}
			finally { conn->Close(); }
		}

		System::Void btnReload_Click(System::Object^ sender, System::EventArgs^ e) {
			txtSearch->Text = "";
			LoadData("");
			ClearFields();
		}

		System::Void btnSearch_Click(System::Object^ sender, System::EventArgs^ e) {
			LoadData(txtSearch->Text);
		}

		System::Void btnAdd_Click(System::Object^ sender, System::EventArgs^ e) {
			ExecuteQuery("INSERT INTO alkos (Type, Brand, Manufacturer, Supplier, ExpirationDate, Price) VALUES (@t, @b, @m, @s, @d, @p)");
		}

		System::Void btnEdit_Click(System::Object^ sender, System::EventArgs^ e) {
			if (currentId == 0) {
				MessageBox::Show("Виберіть рядок для редагування!");
				return;
			}
			ExecuteQuery("UPDATE alkos SET Type=@t, Brand=@b, Manufacturer=@m, Supplier=@s, ExpirationDate=@d, Price=@p WHERE id=" + currentId);
		}

		void ExecuteQuery(String^ sql) {
			if (txtType->Text == "" || txtBrand->Text == "") {
				MessageBox::Show("Заповніть обов'язкові поля!");
				return;
			}
			MySqlConnection^ conn = gcnew MySqlConnection(connString);
			try {
				conn->Open();
				MySqlCommand^ cmd = gcnew MySqlCommand(sql, conn);
				cmd->Parameters->AddWithValue("@t", txtType->Text);
				cmd->Parameters->AddWithValue("@b", txtBrand->Text);
				cmd->Parameters->AddWithValue("@m", txtManuf->Text);
				cmd->Parameters->AddWithValue("@s", txtSupp->Text);
				cmd->Parameters->AddWithValue("@d", dtpDate->Value.ToString("yyyy-MM-dd"));
				cmd->Parameters->AddWithValue("@p", numPrice->Value);
				cmd->ExecuteNonQuery();
				MessageBox::Show("Готово!");
				ClearFields();
				LoadData("");
			}
			catch (Exception^ ex) {
				MessageBox::Show("Помилка: " + ex->Message);
			}
			finally { conn->Close(); }
		}

		System::Void dataGridView1_CellClick(System::Object^ sender, System::Windows::Forms::DataGridViewCellEventArgs^ e) {
			if (e->RowIndex >= 0) {
				DataGridViewRow^ row = dataGridView1->Rows[e->RowIndex];

				// L"id" означає, що це рядок типу String^, а не char*
				currentId = Convert::ToInt32(row->Cells[L"id"]->Value);
				txtType->Text = row->Cells[L"Type"]->Value->ToString();
				txtBrand->Text = row->Cells[L"Brand"]->Value->ToString();
				txtManuf->Text = row->Cells[L"Manufacturer"]->Value->ToString();
				txtSupp->Text = row->Cells[L"Supplier"]->Value->ToString();
				try { dtpDate->Value = Convert::ToDateTime(row->Cells[L"ExpirationDate"]->Value); }
				catch (...) {}
				numPrice->Value = Convert::ToDecimal(row->Cells[L"Price"]->Value);
			}
		}

		void ClearFields() {
			txtType->Text = ""; txtBrand->Text = ""; txtManuf->Text = ""; txtSupp->Text = "";
			currentId = 0;
		}

		System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
			LoadData("");
		}

		void InitializeComponent(void)
		{
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->txtType = (gcnew System::Windows::Forms::TextBox());
			this->txtBrand = (gcnew System::Windows::Forms::TextBox());
			this->txtManuf = (gcnew System::Windows::Forms::TextBox());
			this->txtSupp = (gcnew System::Windows::Forms::TextBox());
			this->txtSearch = (gcnew System::Windows::Forms::TextBox());
			this->dtpDate = (gcnew System::Windows::Forms::DateTimePicker());
			this->numPrice = (gcnew System::Windows::Forms::NumericUpDown());
			this->btnAdd = (gcnew System::Windows::Forms::Button());
			this->btnReload = (gcnew System::Windows::Forms::Button());
			this->btnEdit = (gcnew System::Windows::Forms::Button());
			this->btnSearch = (gcnew System::Windows::Forms::Button());
			this->lblType = (gcnew System::Windows::Forms::Label());
			this->lblBrand = (gcnew System::Windows::Forms::Label());
			this->lblManuf = (gcnew System::Windows::Forms::Label());
			this->lblSupp = (gcnew System::Windows::Forms::Label());
			this->lblDate = (gcnew System::Windows::Forms::Label());
			this->lblPrice = (gcnew System::Windows::Forms::Label());
			this->lblSearch = (gcnew System::Windows::Forms::Label());

			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numPrice))->BeginInit();
			this->SuspendLayout();

			// Таблиця
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->Location = System::Drawing::Point(300, 50);
			this->dataGridView1->Size = System::Drawing::Size(480, 400);

			this->dataGridView1->CellClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MyForm::dataGridView1_CellClick);

			// Пошук
			this->lblSearch->Text = L"Пошук:";
			this->lblSearch->Location = System::Drawing::Point(300, 15);
			this->lblSearch->Size = System::Drawing::Size(50, 20);
			this->txtSearch->Location = System::Drawing::Point(360, 12);
			this->txtSearch->Size = System::Drawing::Size(260, 20);
			this->btnSearch->Text = L"Знайти";
			this->btnSearch->Location = System::Drawing::Point(630, 10);
			this->btnSearch->Size = System::Drawing::Size(100, 25);
			this->btnSearch->Click += gcnew System::EventHandler(this, &MyForm::btnSearch_Click);

			// Поля
			this->lblType->Text = L"Вид:"; this->lblType->Location = System::Drawing::Point(12, 15);
			this->txtType->Location = System::Drawing::Point(100, 12); this->txtType->Size = System::Drawing::Size(180, 20);

			this->lblBrand->Text = L"Марка:"; this->lblBrand->Location = System::Drawing::Point(12, 45);
			this->txtBrand->Location = System::Drawing::Point(100, 42); this->txtBrand->Size = System::Drawing::Size(180, 20);

			this->lblManuf->Text = L"Виробник:"; this->lblManuf->Location = System::Drawing::Point(12, 75);
			this->txtManuf->Location = System::Drawing::Point(100, 72); this->txtManuf->Size = System::Drawing::Size(180, 20);

			this->lblSupp->Text = L"Постачал.:"; this->lblSupp->Location = System::Drawing::Point(12, 105);
			this->txtSupp->Location = System::Drawing::Point(100, 102); this->txtSupp->Size = System::Drawing::Size(180, 20);

			this->lblDate->Text = L"Дата:"; this->lblDate->Location = System::Drawing::Point(12, 135);
			this->dtpDate->Location = System::Drawing::Point(100, 132); this->dtpDate->Size = System::Drawing::Size(180, 20);

			this->lblPrice->Text = L"Ціна:"; this->lblPrice->Location = System::Drawing::Point(12, 165);
			this->numPrice->Location = System::Drawing::Point(100, 162); this->numPrice->Size = System::Drawing::Size(180, 20);
			this->numPrice->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, 0 });

			// Кнопки
			this->btnAdd->Text = L"Додати";
			this->btnAdd->Location = System::Drawing::Point(12, 200);
			this->btnAdd->Size = System::Drawing::Size(270, 35);
			this->btnAdd->Click += gcnew System::EventHandler(this, &MyForm::btnAdd_Click);

			this->btnEdit->Text = L"Зберегти зміни";
			this->btnEdit->Location = System::Drawing::Point(12, 245);
			this->btnEdit->Size = System::Drawing::Size(270, 35);
			this->btnEdit->Click += gcnew System::EventHandler(this, &MyForm::btnEdit_Click);

			this->btnReload->Text = L"Скинути / Оновити";
			this->btnReload->Location = System::Drawing::Point(12, 290);
			this->btnReload->Size = System::Drawing::Size(270, 35);
			this->btnReload->Click += gcnew System::EventHandler(this, &MyForm::btnReload_Click);

			this->ClientSize = System::Drawing::Size(800, 480);
			this->Controls->Add(this->btnSearch); this->Controls->Add(this->txtSearch); this->Controls->Add(this->lblSearch);
			this->Controls->Add(this->btnEdit); this->Controls->Add(this->btnReload); this->Controls->Add(this->btnAdd);
			this->Controls->Add(this->numPrice); this->Controls->Add(this->dtpDate);
			this->Controls->Add(this->txtSupp); this->Controls->Add(this->txtManuf);
			this->Controls->Add(this->txtBrand); this->Controls->Add(this->txtType);
			this->Controls->Add(this->dataGridView1);
			this->Controls->Add(this->lblType); this->Controls->Add(this->lblBrand);
			this->Controls->Add(this->lblManuf); this->Controls->Add(this->lblSupp);
			this->Controls->Add(this->lblDate); this->Controls->Add(this->lblPrice);

			this->Text = L"Лабораторна 4";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);

			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numPrice))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();
		}
	};
}


#include "Functions.h"
#include <windows.h>
#include <atlstr.h>

#pragma once
namespace ImageProcessing
{
	using namespace System;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Collections;
	using namespace System::ComponentModel;
	using namespace System::Windows::Forms;

	BYTE* buffer;
	BYTE* raw_intensity;
	BYTE* crop;
	BYTE* zoom;
	BYTE* equalized;
	BYTE* boundary;
	BYTE* dilation;
	BYTE* erosion;
	BYTE* kmeans;
	BYTE* label;
	BYTE* hysteresishhreshold;
	float* gaussian;
	float* derivative_h;
	float* derivative_v;
	float* arctan;
	float* nonmaxsupression;
	CString path;
	LPCTSTR input;
	int width, widthlimit, height, heightlimit;
	int x_first, y_first, x_last, y_last;
	int loop, count;
	long size;
	double momentcircle[7] = { 0, 0, 0, 0, 0, 0, 0 };
	double momentrectangle[7] = { 0, 0, 0, 0, 0, 0, 0 };
	bool mode = false;       // True: RGB , False: Y
	bool mousedown = false;  // True: Down, False: Up
	bool croporzoom = false; // True: Zoom, False: Crop

	public ref class MainForm : public System::Windows::Forms::Form
	{
		public:
		MainForm(void)
		{
			InitializeComponent();
		}

		protected:
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}

		private: System::ComponentModel::Container ^components;
		private: System::Windows::Forms::MenuStrip^  menustrip;
		private: System::Windows::Forms::ToolStripMenuItem^  menustrip_file;
		private: System::Windows::Forms::ToolStripMenuItem^  menustrip_open;
		private: System::Windows::Forms::ToolStripMenuItem^  menustrip_save;
		private: System::Windows::Forms::ToolStripMenuItem^  menustrip_close;
		private: System::Windows::Forms::ToolStripMenuItem^  menustrip_exit;
		private: System::Windows::Forms::ToolStripMenuItem^  menustrip_convert;
		private: System::Windows::Forms::ToolStripMenuItem^  menustrip_morphological;
		private: System::Windows::Forms::ToolStripMenuItem^  menustrip_morphological_dilation;
		private: System::Windows::Forms::ToolStripMenuItem^  menustrip_morphological_erosion;
		private: System::Windows::Forms::ToolStripMenuItem^  menustrip_morphological_opening;
		private: System::Windows::Forms::ToolStripMenuItem^  menustrip_morphological_closing;
		private: System::Windows::Forms::ToolStripMenuItem^  menustrip_kmeans;
		private: System::Windows::Forms::ToolStripMenuItem^  menustrip_kmeans_euclideon;
		private: System::Windows::Forms::ToolStripMenuItem^  menustrip_kmeans_mahalanobis;
		private: System::Windows::Forms::ToolStripMenuItem^  menustrip_labeling;
		private: System::Windows::Forms::ToolStripMenuItem^  menustrip_image;
		private: System::Windows::Forms::ToolStripMenuItem^  menustrip_crop;
		private: System::Windows::Forms::ToolStripMenuItem^  menustrip_zoom;
		private: System::Windows::Forms::ToolStripMenuItem^  menustrip_histogram;
		private: System::Windows::Forms::ToolStripMenuItem^  menustrip_histogram_normal;
		private: System::Windows::Forms::ToolStripMenuItem^  menustrip_histogram_equalized;
		private: System::Windows::Forms::OpenFileDialog^  openfiledialog;
		private: System::Windows::Forms::SaveFileDialog^  savefiledialog;
		private: System::Windows::Forms::GroupBox^  groupbox_last;
		private: System::Windows::Forms::PictureBox^  picturebox_last;
		private: System::Windows::Forms::Label^  label_mode;
		private: System::Windows::Forms::Button^  button_mode;
		private: System::Windows::Forms::GroupBox^  groupbox_first;
		private: System::Windows::Forms::Panel^  panel_count;
		private: System::Windows::Forms::Button^  button_count_cancel;
		private: System::Windows::Forms::Label^  label_countview;
		private: System::Windows::Forms::Button^  button_count_apply;
		private: System::Windows::Forms::TrackBar^  trackbar_count;
		private: System::Windows::Forms::Label^  label_count;
		private: System::Windows::Forms::DataVisualization::Charting::Chart^  chart_histogram;
		private: System::Windows::Forms::PictureBox^  picturebox_first;
		private: System::Windows::Forms::ToolStripMenuItem^  menustrip_morphological_boundary;
		private: System::Windows::Forms::ToolStripMenuItem^  menustrip_moment;
		private: System::Windows::Forms::ToolStripMenuItem^  menustrip_moment_calculate;
		private: System::Windows::Forms::ToolStripMenuItem^  menustrip_moment_learn;
		private: System::Windows::Forms::ToolStripMenuItem^  menustrip_moment_learn_rectangle;
		private: System::Windows::Forms::ToolStripMenuItem^  menustrip_moment_learn_circle;
		private: System::Windows::Forms::ToolStripMenuItem^  menustrip_edge;
		private: System::Windows::Forms::ToolStripMenuItem^  menustrip_edge_dxdy;
		private: System::Windows::Forms::ToolStripMenuItem^  menustrip_edge_canny;
		private: System::Windows::Forms::ToolStripMenuItem^  menustrip_edge_hough;
		private: System::Windows::Forms::ToolStripMenuItem^  menustrip_sapen;

		#pragma region
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Series^  series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			this->menustrip = (gcnew System::Windows::Forms::MenuStrip());
			this->menustrip_file = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menustrip_open = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menustrip_sapen = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menustrip_close = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menustrip_save = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menustrip_exit = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menustrip_image = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menustrip_crop = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menustrip_zoom = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menustrip_histogram = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menustrip_histogram_normal = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menustrip_histogram_equalized = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menustrip_convert = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menustrip_kmeans = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menustrip_kmeans_euclideon = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menustrip_kmeans_mahalanobis = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menustrip_labeling = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menustrip_morphological = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menustrip_morphological_dilation = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menustrip_morphological_erosion = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menustrip_morphological_opening = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menustrip_morphological_closing = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menustrip_morphological_boundary = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menustrip_moment = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menustrip_moment_learn = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menustrip_moment_learn_circle = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menustrip_moment_learn_rectangle = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menustrip_moment_calculate = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menustrip_edge = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menustrip_edge_dxdy = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menustrip_edge_canny = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menustrip_edge_hough = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->label_mode = (gcnew System::Windows::Forms::Label());
			this->button_mode = (gcnew System::Windows::Forms::Button());
			this->openfiledialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->groupbox_last = (gcnew System::Windows::Forms::GroupBox());
			this->picturebox_last = (gcnew System::Windows::Forms::PictureBox());
			this->savefiledialog = (gcnew System::Windows::Forms::SaveFileDialog());
			this->groupbox_first = (gcnew System::Windows::Forms::GroupBox());
			this->panel_count = (gcnew System::Windows::Forms::Panel());
			this->button_count_cancel = (gcnew System::Windows::Forms::Button());
			this->label_countview = (gcnew System::Windows::Forms::Label());
			this->button_count_apply = (gcnew System::Windows::Forms::Button());
			this->trackbar_count = (gcnew System::Windows::Forms::TrackBar());
			this->label_count = (gcnew System::Windows::Forms::Label());
			this->chart_histogram = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->picturebox_first = (gcnew System::Windows::Forms::PictureBox());
			this->menustrip->SuspendLayout();
			this->groupbox_last->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picturebox_last))->BeginInit();
			this->groupbox_first->SuspendLayout();
			this->panel_count->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackbar_count))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart_histogram))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picturebox_first))->BeginInit();
			this->SuspendLayout();
			// 
			// menustrip
			// 
			this->menustrip->BackColor = System::Drawing::SystemColors::ControlLight;
			this->menustrip->Font = (gcnew System::Drawing::Font(L"OCR A Extended", 10));
			this->menustrip->GripMargin = System::Windows::Forms::Padding(0);
			this->menustrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {
				this->menustrip_file, this->menustrip_image,
					this->menustrip_convert, this->menustrip_moment, this->menustrip_edge
			});
			this->menustrip->LayoutStyle = System::Windows::Forms::ToolStripLayoutStyle::HorizontalStackWithOverflow;
			this->menustrip->Location = System::Drawing::Point(0, 0);
			this->menustrip->Name = L"menustrip";
			this->menustrip->Padding = System::Windows::Forms::Padding(5, 7, 5, 7);
			this->menustrip->Size = System::Drawing::Size(846, 35);
			this->menustrip->TabIndex = 0;
			this->menustrip->Text = L"menustrip";
			// 
			// menustrip_file
			// 
			this->menustrip_file->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {
				this->menustrip_open,
					this->menustrip_sapen, this->menustrip_close, this->menustrip_save, this->menustrip_exit
			});
			this->menustrip_file->Name = L"menustrip_file";
			this->menustrip_file->Padding = System::Windows::Forms::Padding(5, 1, 5, 1);
			this->menustrip_file->Size = System::Drawing::Size(53, 21);
			this->menustrip_file->Text = L"File";
			// 
			// menustrip_open
			// 
			this->menustrip_open->Name = L"menustrip_open";
			this->menustrip_open->Size = System::Drawing::Size(114, 22);
			this->menustrip_open->Text = L"Open";
			this->menustrip_open->Click += gcnew System::EventHandler(this, &MainForm::menustrip_open_Click);
			// 
			// menustrip_sapen
			// 
			this->menustrip_sapen->Enabled = false;
			this->menustrip_sapen->Name = L"menustrip_sapen";
			this->menustrip_sapen->Size = System::Drawing::Size(114, 22);
			this->menustrip_sapen->Text = L"Sapen";
			this->menustrip_sapen->Click += gcnew System::EventHandler(this, &MainForm::menustrip_sapen_Click);
			// 
			// menustrip_close
			// 
			this->menustrip_close->Enabled = false;
			this->menustrip_close->Name = L"menustrip_close";
			this->menustrip_close->Size = System::Drawing::Size(114, 22);
			this->menustrip_close->Text = L"Close";
			this->menustrip_close->Click += gcnew System::EventHandler(this, &MainForm::menustrip_close_Click);
			// 
			// menustrip_save
			// 
			this->menustrip_save->Enabled = false;
			this->menustrip_save->Name = L"menustrip_save";
			this->menustrip_save->Size = System::Drawing::Size(114, 22);
			this->menustrip_save->Text = L"Save";
			this->menustrip_save->Click += gcnew System::EventHandler(this, &MainForm::menustrip_save_Click);
			// 
			// menustrip_exit
			// 
			this->menustrip_exit->Name = L"menustrip_exit";
			this->menustrip_exit->Size = System::Drawing::Size(114, 22);
			this->menustrip_exit->Text = L"Exit";
			this->menustrip_exit->Click += gcnew System::EventHandler(this, &MainForm::menustrip_exit_Click);
			// 
			// menustrip_image
			// 
			this->menustrip_image->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->menustrip_crop,
					this->menustrip_zoom, this->menustrip_histogram
			});
			this->menustrip_image->Enabled = false;
			this->menustrip_image->Name = L"menustrip_image";
			this->menustrip_image->Padding = System::Windows::Forms::Padding(5, 1, 5, 1);
			this->menustrip_image->Size = System::Drawing::Size(61, 21);
			this->menustrip_image->Text = L"Image";
			// 
			// menustrip_crop
			// 
			this->menustrip_crop->Name = L"menustrip_crop";
			this->menustrip_crop->Size = System::Drawing::Size(146, 22);
			this->menustrip_crop->Text = L"Crop";
			this->menustrip_crop->Click += gcnew System::EventHandler(this, &MainForm::menustrip_crop_Click);
			// 
			// menustrip_zoom
			// 
			this->menustrip_zoom->Name = L"menustrip_zoom";
			this->menustrip_zoom->Size = System::Drawing::Size(146, 22);
			this->menustrip_zoom->Text = L"Zoom";
			this->menustrip_zoom->Click += gcnew System::EventHandler(this, &MainForm::menustrip_zoom_Click);
			// 
			// menustrip_histogram
			// 
			this->menustrip_histogram->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->menustrip_histogram_normal,
					this->menustrip_histogram_equalized
			});
			this->menustrip_histogram->Name = L"menustrip_histogram";
			this->menustrip_histogram->Size = System::Drawing::Size(146, 22);
			this->menustrip_histogram->Text = L"Histogram";
			// 
			// menustrip_histogram_normal
			// 
			this->menustrip_histogram_normal->Name = L"menustrip_histogram_normal";
			this->menustrip_histogram_normal->Size = System::Drawing::Size(146, 22);
			this->menustrip_histogram_normal->Text = L"Normal";
			this->menustrip_histogram_normal->Click += gcnew System::EventHandler(this, &MainForm::menustrip_histogram_normal_Click);
			// 
			// menustrip_histogram_equalized
			// 
			this->menustrip_histogram_equalized->Name = L"menustrip_histogram_equalized";
			this->menustrip_histogram_equalized->Size = System::Drawing::Size(146, 22);
			this->menustrip_histogram_equalized->Text = L"Equalized";
			this->menustrip_histogram_equalized->Click += gcnew System::EventHandler(this, &MainForm::menustrip_histogram_equalized_Click);
			// 
			// menustrip_convert
			// 
			this->menustrip_convert->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->menustrip_kmeans,
					this->menustrip_labeling, this->menustrip_morphological
			});
			this->menustrip_convert->Enabled = false;
			this->menustrip_convert->Name = L"menustrip_convert";
			this->menustrip_convert->Padding = System::Windows::Forms::Padding(5, 1, 5, 1);
			this->menustrip_convert->Size = System::Drawing::Size(77, 21);
			this->menustrip_convert->Text = L"Convert";
			// 
			// menustrip_kmeans
			// 
			this->menustrip_kmeans->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->menustrip_kmeans_euclideon,
					this->menustrip_kmeans_mahalanobis
			});
			this->menustrip_kmeans->Name = L"menustrip_kmeans";
			this->menustrip_kmeans->Size = System::Drawing::Size(178, 22);
			this->menustrip_kmeans->Text = L"K-Means";
			// 
			// menustrip_kmeans_euclideon
			// 
			this->menustrip_kmeans_euclideon->Name = L"menustrip_kmeans_euclideon";
			this->menustrip_kmeans_euclideon->Size = System::Drawing::Size(162, 22);
			this->menustrip_kmeans_euclideon->Text = L"Euclideon";
			this->menustrip_kmeans_euclideon->Click += gcnew System::EventHandler(this, &MainForm::menustrip_kmeans_euclideon_Click);
			// 
			// menustrip_kmeans_mahalanobis
			// 
			this->menustrip_kmeans_mahalanobis->Enabled = false;
			this->menustrip_kmeans_mahalanobis->Name = L"menustrip_kmeans_mahalanobis";
			this->menustrip_kmeans_mahalanobis->Size = System::Drawing::Size(162, 22);
			this->menustrip_kmeans_mahalanobis->Text = L"Mahalanobis";
			// 
			// menustrip_labeling
			// 
			this->menustrip_labeling->Name = L"menustrip_labeling";
			this->menustrip_labeling->Size = System::Drawing::Size(178, 22);
			this->menustrip_labeling->Text = L"Labeling";
			this->menustrip_labeling->Click += gcnew System::EventHandler(this, &MainForm::menustrip_labeling_Click);
			// 
			// menustrip_morphological
			// 
			this->menustrip_morphological->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {
				this->menustrip_morphological_dilation,
					this->menustrip_morphological_erosion, this->menustrip_morphological_opening, this->menustrip_morphological_closing, this->menustrip_morphological_boundary
			});
			this->menustrip_morphological->Name = L"menustrip_morphological";
			this->menustrip_morphological->Size = System::Drawing::Size(178, 22);
			this->menustrip_morphological->Text = L"Morphological";
			// 
			// menustrip_morphological_dilation
			// 
			this->menustrip_morphological_dilation->Name = L"menustrip_morphological_dilation";
			this->menustrip_morphological_dilation->Size = System::Drawing::Size(138, 22);
			this->menustrip_morphological_dilation->Text = L"Dilation";
			this->menustrip_morphological_dilation->Click += gcnew System::EventHandler(this, &MainForm::menustrip_morphological_dilation_Click);
			// 
			// menustrip_morphological_erosion
			// 
			this->menustrip_morphological_erosion->Name = L"menustrip_morphological_erosion";
			this->menustrip_morphological_erosion->Size = System::Drawing::Size(138, 22);
			this->menustrip_morphological_erosion->Text = L"Erosion";
			this->menustrip_morphological_erosion->Click += gcnew System::EventHandler(this, &MainForm::menustrip_morphological_erosion_Click);
			// 
			// menustrip_morphological_opening
			// 
			this->menustrip_morphological_opening->Name = L"menustrip_morphological_opening";
			this->menustrip_morphological_opening->Size = System::Drawing::Size(138, 22);
			this->menustrip_morphological_opening->Text = L"Opening";
			this->menustrip_morphological_opening->Click += gcnew System::EventHandler(this, &MainForm::menustrip_morphological_opening_Click);
			// 
			// menustrip_morphological_closing
			// 
			this->menustrip_morphological_closing->Name = L"menustrip_morphological_closing";
			this->menustrip_morphological_closing->Size = System::Drawing::Size(138, 22);
			this->menustrip_morphological_closing->Text = L"Closing";
			this->menustrip_morphological_closing->Click += gcnew System::EventHandler(this, &MainForm::menustrip_morphological_closing_Click);
			// 
			// menustrip_morphological_boundary
			// 
			this->menustrip_morphological_boundary->Name = L"menustrip_morphological_boundary";
			this->menustrip_morphological_boundary->Size = System::Drawing::Size(138, 22);
			this->menustrip_morphological_boundary->Text = L"Boundary";
			this->menustrip_morphological_boundary->Click += gcnew System::EventHandler(this, &MainForm::menustrip_morphological_boundary_Click);
			// 
			// menustrip_moment
			// 
			this->menustrip_moment->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->menustrip_moment_learn,
					this->menustrip_moment_calculate
			});
			this->menustrip_moment->Enabled = false;
			this->menustrip_moment->Name = L"menustrip_moment";
			this->menustrip_moment->Size = System::Drawing::Size(67, 21);
			this->menustrip_moment->Text = L"Moment";
			// 
			// menustrip_moment_learn
			// 
			this->menustrip_moment_learn->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->menustrip_moment_learn_circle,
					this->menustrip_moment_learn_rectangle
			});
			this->menustrip_moment_learn->Name = L"menustrip_moment_learn";
			this->menustrip_moment_learn->Size = System::Drawing::Size(146, 22);
			this->menustrip_moment_learn->Text = L"Learn";
			// 
			// menustrip_moment_learn_circle
			// 
			this->menustrip_moment_learn_circle->Name = L"menustrip_moment_learn_circle";
			this->menustrip_moment_learn_circle->Size = System::Drawing::Size(146, 22);
			this->menustrip_moment_learn_circle->Text = L"Circle";
			this->menustrip_moment_learn_circle->Click += gcnew System::EventHandler(this, &MainForm::menustrip_moment_learn_circle_Click);
			// 
			// menustrip_moment_learn_rectangle
			// 
			this->menustrip_moment_learn_rectangle->Name = L"menustrip_moment_learn_rectangle";
			this->menustrip_moment_learn_rectangle->Size = System::Drawing::Size(146, 22);
			this->menustrip_moment_learn_rectangle->Text = L"Rectangle";
			this->menustrip_moment_learn_rectangle->Click += gcnew System::EventHandler(this, &MainForm::menustrip_moment_learn_rectangle_Click);
			// 
			// menustrip_moment_calculate
			// 
			this->menustrip_moment_calculate->Name = L"menustrip_moment_calculate";
			this->menustrip_moment_calculate->Size = System::Drawing::Size(146, 22);
			this->menustrip_moment_calculate->Text = L"Calculate";
			this->menustrip_moment_calculate->Click += gcnew System::EventHandler(this, &MainForm::menustrip_moment_calculate_Click);
			// 
			// menustrip_edge
			// 
			this->menustrip_edge->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->menustrip_edge_dxdy,
					this->menustrip_edge_canny, this->menustrip_edge_hough
			});
			this->menustrip_edge->Enabled = false;
			this->menustrip_edge->Name = L"menustrip_edge";
			this->menustrip_edge->Size = System::Drawing::Size(51, 21);
			this->menustrip_edge->Text = L"Edge";
			// 
			// menustrip_edge_dxdy
			// 
			this->menustrip_edge_dxdy->Name = L"menustrip_edge_dxdy";
			this->menustrip_edge_dxdy->Size = System::Drawing::Size(180, 22);
			this->menustrip_edge_dxdy->Text = L"Dx + Dy";
			this->menustrip_edge_dxdy->Click += gcnew System::EventHandler(this, &MainForm::menustrip_edge_dxdy_Click);
			// 
			// menustrip_edge_canny
			// 
			this->menustrip_edge_canny->Name = L"menustrip_edge_canny";
			this->menustrip_edge_canny->Size = System::Drawing::Size(180, 22);
			this->menustrip_edge_canny->Text = L"Canny";
			this->menustrip_edge_canny->Click += gcnew System::EventHandler(this, &MainForm::menustrip_edge_canny_Click);
			// 
			// menustrip_edge_hough
			// 
			this->menustrip_edge_hough->Name = L"menustrip_edge_hough";
			this->menustrip_edge_hough->Size = System::Drawing::Size(180, 22);
			this->menustrip_edge_hough->Text = L"Hough";
			// 
			// label_mode
			// 
			this->label_mode->BackColor = System::Drawing::SystemColors::ControlDark;
			this->label_mode->Font = (gcnew System::Drawing::Font(L"OCR A Extended", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label_mode->ForeColor = System::Drawing::Color::White;
			this->label_mode->Location = System::Drawing::Point(746, 0);
			this->label_mode->Margin = System::Windows::Forms::Padding(0);
			this->label_mode->Name = L"label_mode";
			this->label_mode->Size = System::Drawing::Size(100, 35);
			this->label_mode->TabIndex = 4;
			this->label_mode->Text = L"Mode: Y";
			this->label_mode->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// button_mode
			// 
			this->button_mode->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->button_mode->BackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->button_mode->Cursor = System::Windows::Forms::Cursors::Hand;
			this->button_mode->Enabled = false;
			this->button_mode->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button_mode->ForeColor = System::Drawing::SystemColors::ControlDarkDark;
			this->button_mode->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"button_mode.Image")));
			this->button_mode->Location = System::Drawing::Point(711, 0);
			this->button_mode->Margin = System::Windows::Forms::Padding(0);
			this->button_mode->Name = L"button_mode";
			this->button_mode->Size = System::Drawing::Size(35, 35);
			this->button_mode->TabIndex = 5;
			this->button_mode->UseVisualStyleBackColor = false;
			this->button_mode->Click += gcnew System::EventHandler(this, &MainForm::button_mode_Click);
			// 
			// openfiledialog
			// 
			this->openfiledialog->Filter = L"bmp files (*.bmp)|*.bmp";
			this->openfiledialog->Title = L"Open Image";
			this->openfiledialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MainForm::menustrip_open_ok_Click);
			// 
			// groupbox_last
			// 
			this->groupbox_last->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->groupbox_last->BackColor = System::Drawing::Color::Transparent;
			this->groupbox_last->Controls->Add(this->picturebox_last);
			this->groupbox_last->Font = (gcnew System::Drawing::Font(L"OCR A Extended", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->groupbox_last->Location = System::Drawing::Point(428, 40);
			this->groupbox_last->Margin = System::Windows::Forms::Padding(5);
			this->groupbox_last->Name = L"groupbox_last";
			this->groupbox_last->Padding = System::Windows::Forms::Padding(10);
			this->groupbox_last->Size = System::Drawing::Size(404, 418);
			this->groupbox_last->TabIndex = 3;
			this->groupbox_last->TabStop = false;
			this->groupbox_last->Text = L"Group Box Last";
			this->groupbox_last->Visible = false;
			// 
			// picturebox_last
			// 
			this->picturebox_last->BackColor = System::Drawing::SystemColors::Control;
			this->picturebox_last->Location = System::Drawing::Point(10, 26);
			this->picturebox_last->Margin = System::Windows::Forms::Padding(0);
			this->picturebox_last->Name = L"picturebox_last";
			this->picturebox_last->Size = System::Drawing::Size(384, 384);
			this->picturebox_last->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
			this->picturebox_last->TabIndex = 2;
			this->picturebox_last->TabStop = false;
			// 
			// savefiledialog
			// 
			this->savefiledialog->Filter = L"bmp files (*.bmp)|*.bmp";
			this->savefiledialog->Title = L"Save Image";
			this->savefiledialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MainForm::menustrip_save_ok_Click);
			// 
			// groupbox_first
			// 
			this->groupbox_first->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->groupbox_first->BackColor = System::Drawing::Color::Transparent;
			this->groupbox_first->Controls->Add(this->panel_count);
			this->groupbox_first->Controls->Add(this->chart_histogram);
			this->groupbox_first->Controls->Add(this->picturebox_first);
			this->groupbox_first->Font = (gcnew System::Drawing::Font(L"OCR A Extended", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->groupbox_first->Location = System::Drawing::Point(14, 40);
			this->groupbox_first->Margin = System::Windows::Forms::Padding(5);
			this->groupbox_first->Name = L"groupbox_first";
			this->groupbox_first->Padding = System::Windows::Forms::Padding(10);
			this->groupbox_first->Size = System::Drawing::Size(404, 418);
			this->groupbox_first->TabIndex = 1;
			this->groupbox_first->TabStop = false;
			this->groupbox_first->Text = L"Group Box First";
			this->groupbox_first->Visible = false;
			// 
			// panel_count
			// 
			this->panel_count->Controls->Add(this->button_count_cancel);
			this->panel_count->Controls->Add(this->label_countview);
			this->panel_count->Controls->Add(this->button_count_apply);
			this->panel_count->Controls->Add(this->trackbar_count);
			this->panel_count->Controls->Add(this->label_count);
			this->panel_count->Location = System::Drawing::Point(10, 24);
			this->panel_count->Margin = System::Windows::Forms::Padding(0);
			this->panel_count->Name = L"panel_count";
			this->panel_count->Size = System::Drawing::Size(384, 384);
			this->panel_count->TabIndex = 4;
			this->panel_count->Visible = false;
			// 
			// button_count_cancel
			// 
			this->button_count_cancel->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->button_count_cancel->Cursor = System::Windows::Forms::Cursors::Hand;
			this->button_count_cancel->Font = (gcnew System::Drawing::Font(L"OCR A Extended", 15, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button_count_cancel->Location = System::Drawing::Point(96, 295);
			this->button_count_cancel->Margin = System::Windows::Forms::Padding(96, 8, 96, 0);
			this->button_count_cancel->Name = L"button_count_cancel";
			this->button_count_cancel->Size = System::Drawing::Size(192, 48);
			this->button_count_cancel->TabIndex = 4;
			this->button_count_cancel->Text = L"Cancel";
			this->button_count_cancel->UseVisualStyleBackColor = true;
			this->button_count_cancel->Click += gcnew System::EventHandler(this, &MainForm::button_dotcount_cancel_Click);
			// 
			// label_countview
			// 
			this->label_countview->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->label_countview->Font = (gcnew System::Drawing::Font(L"OCR A Extended", 20, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label_countview->Location = System::Drawing::Point(160, 82);
			this->label_countview->Margin = System::Windows::Forms::Padding(160, 32, 160, 32);
			this->label_countview->Name = L"label_countview";
			this->label_countview->Size = System::Drawing::Size(64, 64);
			this->label_countview->TabIndex = 2;
			this->label_countview->Text = L"2";
			this->label_countview->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// button_count_apply
			// 
			this->button_count_apply->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->button_count_apply->Cursor = System::Windows::Forms::Cursors::Hand;
			this->button_count_apply->Font = (gcnew System::Drawing::Font(L"OCR A Extended", 15, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button_count_apply->Location = System::Drawing::Point(96, 239);
			this->button_count_apply->Margin = System::Windows::Forms::Padding(96, 16, 96, 0);
			this->button_count_apply->Name = L"button_count_apply";
			this->button_count_apply->Size = System::Drawing::Size(192, 48);
			this->button_count_apply->TabIndex = 3;
			this->button_count_apply->Text = L"Apply";
			this->button_count_apply->UseVisualStyleBackColor = true;
			this->button_count_apply->Click += gcnew System::EventHandler(this, &MainForm::button_dotcount_apply_Click);
			// 
			// trackbar_count
			// 
			this->trackbar_count->BackColor = System::Drawing::SystemColors::Control;
			this->trackbar_count->Cursor = System::Windows::Forms::Cursors::Hand;
			this->trackbar_count->LargeChange = 1;
			this->trackbar_count->Location = System::Drawing::Point(64, 178);
			this->trackbar_count->Margin = System::Windows::Forms::Padding(64, 0, 64, 0);
			this->trackbar_count->Maximum = 20;
			this->trackbar_count->Minimum = 2;
			this->trackbar_count->Name = L"trackbar_count";
			this->trackbar_count->Size = System::Drawing::Size(256, 45);
			this->trackbar_count->TabIndex = 1;
			this->trackbar_count->TabStop = false;
			this->trackbar_count->TickStyle = System::Windows::Forms::TickStyle::Both;
			this->trackbar_count->Value = 2;
			this->trackbar_count->ValueChanged += gcnew System::EventHandler(this, &MainForm::trackbar_dotcount_ValueChanged);
			// 
			// label_count
			// 
			this->label_count->Dock = System::Windows::Forms::DockStyle::Top;
			this->label_count->Font = (gcnew System::Drawing::Font(L"OCR A Extended", 20, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label_count->Location = System::Drawing::Point(0, 0);
			this->label_count->Margin = System::Windows::Forms::Padding(0);
			this->label_count->Name = L"label_count";
			this->label_count->Size = System::Drawing::Size(384, 50);
			this->label_count->TabIndex = 0;
			this->label_count->Text = L"Select Count";
			this->label_count->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// chart_histogram
			// 
			this->chart_histogram->BackColor = System::Drawing::SystemColors::Control;
			chartArea1->Area3DStyle->IsRightAngleAxes = false;
			chartArea1->Area3DStyle->PointDepth = 200;
			chartArea1->Area3DStyle->PointGapDepth = 200;
			chartArea1->Area3DStyle->WallWidth = 5;
			chartArea1->AxisX->ArrowStyle = System::Windows::Forms::DataVisualization::Charting::AxisArrowStyle::Triangle;
			chartArea1->AxisX->IntervalAutoMode = System::Windows::Forms::DataVisualization::Charting::IntervalAutoMode::VariableCount;
			chartArea1->AxisX->IsLabelAutoFit = false;
			chartArea1->AxisX->LabelAutoFitMinFontSize = 5;
			chartArea1->AxisX->TitleFont = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 5));
			chartArea1->AxisY->ArrowStyle = System::Windows::Forms::DataVisualization::Charting::AxisArrowStyle::Triangle;
			chartArea1->AxisY->IntervalAutoMode = System::Windows::Forms::DataVisualization::Charting::IntervalAutoMode::VariableCount;
			chartArea1->AxisY->IsInterlaced = true;
			chartArea1->AxisY->IsLabelAutoFit = false;
			chartArea1->AxisY->LabelAutoFitMinFontSize = 5;
			chartArea1->AxisY->TitleFont = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 5));
			chartArea1->BackColor = System::Drawing::SystemColors::Control;
			chartArea1->Name = L"ChartArea";
			this->chart_histogram->ChartAreas->Add(chartArea1);
			this->chart_histogram->Location = System::Drawing::Point(10, 24);
			this->chart_histogram->Margin = System::Windows::Forms::Padding(0);
			this->chart_histogram->Name = L"chart_histogram";
			series1->ChartArea = L"ChartArea";
			series1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 5));
			series1->Name = L"Value";
			this->chart_histogram->Series->Add(series1);
			this->chart_histogram->Size = System::Drawing::Size(384, 384);
			this->chart_histogram->TabIndex = 0;
			this->chart_histogram->Text = L"Histogram";
			// 
			// picturebox_first
			// 
			this->picturebox_first->BackColor = System::Drawing::SystemColors::Control;
			this->picturebox_first->Location = System::Drawing::Point(10, 24);
			this->picturebox_first->Margin = System::Windows::Forms::Padding(0);
			this->picturebox_first->Name = L"picturebox_first";
			this->picturebox_first->Size = System::Drawing::Size(384, 384);
			this->picturebox_first->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
			this->picturebox_first->TabIndex = 1;
			this->picturebox_first->TabStop = false;
			this->picturebox_first->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::picturebox_first_MouseDown);
			this->picturebox_first->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::picturebox_first_MouseMove);
			this->picturebox_first->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::picturebox_first_MouseUp);
			// 
			// MainForm
			// 
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::None;
			this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->ClientSize = System::Drawing::Size(846, 472);
			this->Controls->Add(this->groupbox_first);
			this->Controls->Add(this->groupbox_last);
			this->Controls->Add(this->button_mode);
			this->Controls->Add(this->label_mode);
			this->Controls->Add(this->menustrip);
			this->Cursor = System::Windows::Forms::Cursors::Default;
			this->DoubleBuffered = true;
			this->Font = (gcnew System::Drawing::Font(L"OCR A Extended", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->MainMenuStrip = this->menustrip;
			this->Name = L"MainForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Image Processing";
			this->menustrip->ResumeLayout(false);
			this->menustrip->PerformLayout();
			this->groupbox_last->ResumeLayout(false);
			this->groupbox_last->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picturebox_last))->EndInit();
			this->groupbox_first->ResumeLayout(false);
			this->groupbox_first->PerformLayout();
			this->panel_count->ResumeLayout(false);
			this->panel_count->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackbar_count))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart_histogram))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picturebox_first))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
		#pragma endregion

		private: System::Void UIChange(bool gb_first, bool gb_last, String^ gb_first_text, String^ gb_last_text, bool pb_last, bool p_count, bool c_histogram, bool lock)
		{
			// Group Box Visible
			groupbox_first->Visible = gb_first;
			groupbox_last->Visible = gb_last;
			// Group Box Text
			groupbox_first->Text = gb_first_text;
			groupbox_last->Text = gb_last_text;
			// Picture Box Enabled
			picturebox_first->Enabled = pb_last;
			// Panel Count Visible
			panel_count->Visible = p_count;
			// Chart Histogram Visible
			chart_histogram->Visible = c_histogram;
			// Button Enabled
			button_mode->Enabled = lock;
			// Menu Strip Enabled
			menustrip_sapen->Enabled = lock;
			menustrip_close->Enabled = lock;
			menustrip_save->Enabled = lock;
			menustrip_convert->Enabled = lock;
			menustrip_image->Enabled = lock;
			menustrip_moment->Enabled = lock;
			menustrip_edge->Enabled = lock;
		}
					
		private: System::Void button_mode_Click(System::Object^  sender, System::EventArgs^  e) 
		{
			System::ComponentModel::CancelEventArgs^ v;

			if (mode)
			{
				mode = false;
				label_mode->Text = "Mode: Y";
				menustrip_open_ok_Click(sender, v);
				
			}
			else
			{
				mode = true;
				label_mode->Text = "Mode: RGB";
				menustrip_open_ok_Click(sender, v);
			}
		}

		private: System::Void menustrip_open_Click(System::Object^  sender, System::EventArgs^  e)
		{
			openfiledialog->ShowDialog();
		}

		private: System::Void menustrip_open_ok_Click(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e)
		{
			// Size Limit Set
			widthlimit = 384;
			heightlimit = 384;
			// Main Operations
			path = openfiledialog->FileName;
			input = (LPCTSTR)path;
			buffer = LoadBMP(&width, &height, &size, input);
			// Image Size Control
			if (width > widthlimit || height > heightlimit)
			{
				String^ text = "Width or height of the picture exceeds the limit! Maximum width and height " + widthlimit + " pixels";
				String^ caption = "Image Size Warning!";
				MessageBoxButtons button = MessageBoxButtons::OK;
				MessageBoxIcon icon = MessageBoxIcon::Information;
				MessageBox::Show(this, text, caption, button, icon);
				// UI Change
				UIChange(false, false, "", "", false, false, false, false);
			}
			else
			{
				picturebox_first->ImageLocation = openfiledialog->FileName;
				//SurfaceSetPixelRGB(buffer, picturebox_first, width, height);
				raw_intensity = BMPToIntensity(buffer, width, height);
				SurfaceSetPixel(raw_intensity, picturebox_last, width, height);
				// UI Change
				UIChange(true, true, "RGB", "Y", false, false, false, true);
			}
		}

		private: System::Void menustrip_sapen_Click(System::Object^  sender, System::EventArgs^  e)
		{
			// Save Image
			Bitmap^ bmp = gcnew Bitmap(picturebox_last->Width, picturebox_last->Height, System::Drawing::Imaging::PixelFormat::Format24bppRgb);
			picturebox_last->DrawToBitmap(bmp, picturebox_last->ClientRectangle);
			bmp->Save(openfiledialog->FileName->Substring(0, openfiledialog->FileName->Length - 4) + "_" + groupbox_last->Text + ".bmp", System::Drawing::Imaging::ImageFormat::Bmp);
			// Load Image
			path = openfiledialog->FileName->Substring(0, openfiledialog->FileName->Length - 4) + "_"  + groupbox_last->Text + ".bmp";
			input = (LPCTSTR)path;
			buffer = LoadBMP(&width, &height, &size, input);
			picturebox_first->ImageLocation = openfiledialog->FileName->Substring(0, openfiledialog->FileName->Length - 4) + "_" + groupbox_last->Text + ".bmp";
			//SurfaceSetPixelRGB(buffer, picturebox_first, width, height);
			raw_intensity = BMPToIntensity(buffer, width, height);
			SurfaceSetPixel(raw_intensity, picturebox_last, width, height);
			// UI Change
			UIChange(true, true, "Source", "Y", false, false, false, true);
		}

		private: System::Void menustrip_close_Click(System::Object^  sender, System::EventArgs^  e)
		{
			buffer = nullptr;
			raw_intensity = nullptr;
			crop = nullptr;
			zoom = nullptr;
			kmeans = nullptr;
			equalized = nullptr;
			// UI Change
			UIChange(false, false, "", "", false, false, false, false);
		}

		private: System::Void menustrip_save_Click(System::Object^  sender, System::EventArgs^  e)
		{
			savefiledialog->ShowDialog();
		}

		private: System::Void menustrip_save_ok_Click(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) 
		{
			// Main Operations
			Bitmap^ bmp = gcnew Bitmap(picturebox_last->Width, picturebox_last->Height, System::Drawing::Imaging::PixelFormat::Format24bppRgb);
			picturebox_last->DrawToBitmap(bmp, picturebox_last->ClientRectangle);
			bmp->Save(savefiledialog->FileName, System::Drawing::Imaging::ImageFormat::Bmp);
		}

		private: System::Void menustrip_exit_Click(System::Object^  sender, System::EventArgs^  e)
		{
			Application::Exit();
		}

		private: System::Void menustrip_crop_Click(System::Object^  sender, System::EventArgs^  e)
		{
			// Picture Box Refresh
			raw_intensity = BMPToIntensity(buffer, width, height);
			SurfaceSetPixel(raw_intensity, picturebox_first, width, height);
			// Main Operations
			picturebox_last->Image = nullptr;
			croporzoom = false;
			// UI Change
			UIChange(true, true, "Y", "Crop", true, false, false, true);
		}

		private: System::Void menustrip_zoom_Click(System::Object^  sender, System::EventArgs^  e) 
		{
			// Picture Box Refresh
			raw_intensity = BMPToIntensity(buffer, width, height);
			SurfaceSetPixel(raw_intensity, picturebox_first, width, height);
			// Main Operations
			picturebox_last->Image = nullptr;
			croporzoom = true;
			// UI Change
			UIChange(true, true, "Y", "Zoom", true, false, false, true);
		}

		private: System::Void picturebox_first_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		{
			// First Coordinate Set
			x_first = System::Convert::ToInt32(e->X);
			y_first = System::Convert::ToInt32(e->Y);
			// Mouse Down Set
			mousedown = true;
		}

		private: System::Void picturebox_first_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
		{
			// Pen and Graphics Set
			Pen^ pen = gcnew Pen(Color::White, 1);;
			Graphics^ graphics;
			// Last Coordinate Control and Set
			if (croporzoom)
			{
				if (e->X > x_first + (widthlimit / 2))
					x_last = x_first + (widthlimit / 2);
				else
					x_last = System::Convert::ToInt32(e->X);
				if (e->X > width && width - x_first < (widthlimit / 2))
					x_last = width;
				if (e->Y > y_first + (heightlimit / 2))
					y_last = y_first + (heightlimit / 2);
				else
					y_last = System::Convert::ToInt32(e->Y);
				if (e->Y > height && height - y_first < (heightlimit / 2))
					y_last = height;
			}
			else
			{
				if (e->X > width)
					x_last = width;
				else
					x_last = System::Convert::ToInt32(e->X);
				if (e->Y > height)
					y_last = height;
				else
					y_last = System::Convert::ToInt32(e->Y);
			}
			// Picture Box Draw Rectanble
			if (mousedown)
			{
				// Picture Box Refresh
				picturebox_first->Refresh();
				// Draw Rectangle
				graphics = picturebox_first->CreateGraphics();
				graphics->DrawRectangle(pen, x_first, y_first, x_last - x_first, y_last - y_first);
			}
		}

		private: System::Void picturebox_first_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		{
			// Summary Variable
			int CWidth = x_last - x_first;
			int CHeight = y_last - y_first;
			// Rectangle Size Test
			if (CWidth > 0 && CHeight > 0)
			{
				if (mode)
					crop = CropImageRGB(buffer, width, height, x_first, y_first, x_last, y_last);
				else
					crop = CropImage(raw_intensity, width, height, x_first, y_first, x_last, y_last);
				// Crop or Zoom Select
				if (croporzoom)
				{
					zoom = ZoomImage(crop, CWidth, CHeight);
					SurfaceSetPixel(zoom, picturebox_last, CWidth * 2 - 1, CHeight * 2 - 1);
				}
				else 
				{
					if (mode)
						SurfaceSetPixelRGB(crop, picturebox_last, CWidth, CHeight);
					else
						SurfaceSetPixel(crop, picturebox_last, CWidth, CHeight);
				}
			}
			// Mouse Down Set
			mousedown = false;
		}

		private: System::Void menustrip_histogram_normal_Click(System::Object^  sender, System::EventArgs^  e) 
		{
			// Picture Box Refresh
			raw_intensity = BMPToIntensity(buffer, width, height);
			SurfaceSetPixel(raw_intensity, picturebox_last, width, height);
			// Main Operations
			HistogramDraw(raw_intensity, chart_histogram, width, height);
			// UI Change
			UIChange(true, true, "Histogram", "Y", false, false, true, true);
		}

		private: System::Void menustrip_histogram_equalized_Click(System::Object^  sender, System::EventArgs^  e) 
		{
			// Picture Box Refresh
			raw_intensity = BMPToIntensity(buffer, width, height);
			SurfaceSetPixel(raw_intensity, picturebox_last, width, height);
			// Main Operations
			equalized = HistogramEqualized(raw_intensity, width, height);
			SurfaceSetPixel(equalized, picturebox_last, width, height);
			HistogramDraw(equalized, chart_histogram, width, height);
			// UI Change
			UIChange(true, true, "Histogram", "Y Equalized", false, false, true, true);
		}

		private: System::Void menustrip_morphological_dilation_Click(System::Object^  sender, System::EventArgs^  e)
		{
			// UI Change
			UIChange(true, true, "Binary", "Dilation", false, true, false, true);
		}

		private: System::Void menustrip_morphological_erosion_Click(System::Object^  sender, System::EventArgs^  e)
		{
			// UI Change
			UIChange(true, true, "Binary", "Erosion", false, true, false, true);
		}

		private: System::Void menustrip_morphological_boundary_Click(System::Object^  sender, System::EventArgs^  e)
		{
			// UI Change
			UIChange(true, true, "Binary", "Boundary", false, true, false, true);
		}

		private: System::Void menustrip_morphological_opening_Click(System::Object^  sender, System::EventArgs^  e)
		{
			// UI Change
			UIChange(true, true, "Binary", "Opening", false, true, false, true);
		}

		private: System::Void menustrip_morphological_closing_Click(System::Object^  sender, System::EventArgs^  e)
		{
			// UI Change
			UIChange(true, true, "Binary", "Closing", false, true, false, true);
		}

		private: System::Void menustrip_kmeans_euclideon_Click(System::Object^  sender, System::EventArgs^  e)
		{
			// Picture Box Refresh
			raw_intensity = BMPToIntensity(buffer, width, height);
			SurfaceSetPixel(raw_intensity, picturebox_first, width, height);
			// UI Change
			UIChange(true, true, "Y", "K-Means", false, true, false, true);
		}

		private: System::Void trackbar_dotcount_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		{
			label_countview->Text = trackbar_count->Value.ToString();
		}

		private: System::Void button_dotcount_apply_Click(System::Object^  sender, System::EventArgs^  e)
		{
			if (groupbox_last->Text == "K-Means")
			{
				// Main Operations
				kmeans = KMeans(raw_intensity, width, height, trackbar_count->Value, &loop);
				SurfaceSetPixel(kmeans, picturebox_last, width, height);
				// Loop Control
				if (loop >= 1000)
				{
					String^ text = "K-Means cycle turned 1,000 rounds but did not find the end result.";
					String^ caption = "K-Means Loop Warning!";
					MessageBoxButtons buttons = MessageBoxButtons::OK;
					MessageBoxIcon icon = MessageBoxIcon::Warning;
					MessageBox::Show(this, text, caption, buttons, icon);
				}
				// UI Change
				UIChange(true, true, "Y", "K-Means", false, false, false, true);
			}
			else if (groupbox_last->Text == "Dilation")
			{
				// Image Binary
				kmeans = KMeans(raw_intensity, width, height, 2, &loop);
				SurfaceSetPixel(kmeans, picturebox_first, width, height);
				// Dilation
				dilation = Dilation(kmeans, width, height);
				for (int i = 0; i < (trackbar_count->Value - 1); i++)
					dilation = Dilation(dilation, width, height);
				SurfaceSetPixel(dilation, picturebox_last, width, height);
				// UI Change
				UIChange(true, true, "Binary", "Dilation", false, false, false, true);
			}
			else if (groupbox_last->Text == "Erosion")
			{
				// Image Binary
				kmeans = KMeans(raw_intensity, width, height, 2, &loop);
				SurfaceSetPixel(kmeans, picturebox_first, width, height);
				// Erosion
				erosion = Erosion(kmeans, width, height);
				for (int i = 0; i < (trackbar_count->Value - 1); i++)
					erosion = Erosion(erosion, width, height);
				SurfaceSetPixel(erosion, picturebox_last, width, height);
				// UI Change
				UIChange(true, true, "Binary", "Erosion", false, false, false, true);
			}
			else if (groupbox_last->Text == "Boundary")
			{
				// Image Binary
				kmeans = KMeans(raw_intensity, width, height, 2, &loop);
				SurfaceSetPixel(kmeans, picturebox_first, width, height);
				// Boundary
				boundary = Boundary(kmeans, width, height, trackbar_count->Value - 1);
				SurfaceSetPixel(boundary, picturebox_last, width, height);
				// UI Change
				UIChange(true, true, "Binary", "Boundary", false, false, false, true);
			}
			else if (groupbox_last->Text == "Opening")
			{
				// Image Binary
				kmeans = KMeans(raw_intensity, width, height, 2, &loop);
				SurfaceSetPixel(kmeans, picturebox_first, width, height);
				// Erosion
				erosion = Erosion(kmeans, width, height);
				for (int i = 0; i < (trackbar_count->Value - 1); i++)
					erosion = Erosion(erosion, width, height);
				// Dilation
				dilation = Dilation(erosion, width, height);
				for (int i = 0; i < (trackbar_count->Value - 1); i++)
					dilation = Dilation(dilation, width, height);
				SurfaceSetPixel(dilation, picturebox_last, width, height);
				// UI Change
				UIChange(true, true, "Binary", "Opening", false, false, false, true);
			}
			else if (groupbox_last->Text == "Closing")
			{
				// Image Binary
				kmeans = KMeans(raw_intensity, width, height, 2, &loop);
				SurfaceSetPixel(kmeans, picturebox_first, width, height);
				// Dilation
				dilation = Dilation(kmeans, width, height);
				for (int i = 0; i < (trackbar_count->Value - 1); i++)
					dilation = Dilation(dilation, width, height);
				// Erosion
				erosion = Erosion(dilation, width, height);
				for (int i = 0; i < (trackbar_count->Value - 1); i++)
					erosion = Erosion(erosion, width, height);
				SurfaceSetPixel(erosion, picturebox_last, width, height);
				// UI Change
				UIChange(true, true, "Binary", "Closing", false, false, false, true);
			}
		}

		private: System::Void button_dotcount_cancel_Click(System::Object^  sender, System::EventArgs^  e)
		{
			System::ComponentModel::CancelEventArgs^ f;
			// Main Operations
			menustrip_open_ok_Click(sender, f);
		}

		private: System::Void menustrip_labeling_Click(System::Object^  sender, System::EventArgs^  e)
		{
			// Image Binary
			kmeans = KMeans(raw_intensity, width, height, 2, &loop);
			SurfaceSetPixel(kmeans, picturebox_first, width, height);
			// Labeling
			label = Labeling(kmeans, width, height, &count);
			SurfaceSetPixel(label, picturebox_last, width, height);
			// Count Print
			String^ text = "There are " + count + " figures in the picture.";
			String^ caption = "Labeling Result";
			MessageBoxButtons buttons = MessageBoxButtons::OK;
			MessageBoxIcon icon = MessageBoxIcon::Information;
			MessageBox::Show(this, text, caption, buttons, icon);
			// UI Change
			UIChange(true, true, "Binary", "Labeling", false, false, false, true);
		}

		private: System::Void menustrip_moment_calculate_Click(System::Object^  sender, System::EventArgs^  e) 
		{
			// Image Binary
			kmeans = KMeans(raw_intensity, width, height, 2, &loop);
			SurfaceSetPixel(kmeans, picturebox_first, width, height);
			// Main Operations
			double* IM = IMoments(kmeans, width, height);
			double tempcircle = 0;
			double temprectangle = 0;
			String^ shape;

			for (int i = 0; i < 7; i++)
			{
				if (Math::Abs(IM[i] - momentcircle[i]) < 0.0001F)
					tempcircle++;
				if (Math::Abs(IM[i] - momentrectangle[i]) < 0.0001F)
					temprectangle++;
			}

			if (tempcircle > temprectangle)
				shape = "Circle";
			else if (temprectangle > tempcircle)
				shape = "Rectangle";
			else
				shape = "Not Found!";

			// Count Print
			String^ text = "I1: " + IM[0] + "\nI2: " + IM[1] + "\nI3: " + IM[2] + "\nI4: " + IM[3] + "\nI5: " + IM[4] + "\nI6: " + IM[5] + "\nI7: " + IM[6] + "\nGuess Shape: " + shape;
			String^ caption = "Moment Result";
			MessageBoxButtons buttons = MessageBoxButtons::OK;
			MessageBoxIcon icon = MessageBoxIcon::Information;
			MessageBox::Show(this, text, caption, buttons, icon);
		}

		private: System::Void menustrip_moment_learn_circle_Click(System::Object^  sender, System::EventArgs^  e)
		{
			// Image Binary
			kmeans = KMeans(raw_intensity, width, height, 2, &loop);
			SurfaceSetPixel(kmeans, picturebox_first, width, height);
			// Main Operations
			double* IM = IMoments(kmeans, width, height);

			for (int i = 0; i < 7; i++)
				momentcircle[i] += IM[i];

			if (momentcircle[0] != IM[0])
			{
				for (int i = 0; i < 7; i++)
					momentcircle[i] /= 2;
			}

			// Succesful Print
			String^ text = "I1: " + momentcircle[0] + "\nI2: " + momentcircle[1] + "\nI3: " + momentcircle[2] + "\nI4: " + momentcircle[3] + "\nI5: " + momentcircle[4] + "\nI6: " + momentcircle[5] + "\nI7: " + momentcircle[6];
			String^ caption = "Learn Circle";
			MessageBoxButtons buttons = MessageBoxButtons::OK;
			MessageBoxIcon icon = MessageBoxIcon::Information;
			MessageBox::Show(this, text, caption, buttons, icon);
		}

		private: System::Void menustrip_moment_learn_rectangle_Click(System::Object^  sender, System::EventArgs^  e) 
		{
			// Image Binary
			kmeans = KMeans(raw_intensity, width, height, 2, &loop);
			SurfaceSetPixel(kmeans, picturebox_first, width, height);
			// Main Operations
			double* IM = IMoments(kmeans, width, height);

			for (int i = 0; i < 7; i++)
				momentrectangle[i] += IM[i];

			if (momentrectangle[0] != IM[0])
			{
				for (int i = 0; i < 7; i++)
					momentrectangle[i] /= 2;
			}

			// Succesful Print
			String^ text = "I1: " + momentrectangle[0] + "\nI2: " + momentrectangle[1] + "\nI3: " + momentrectangle[2] + "\nI4: " + momentrectangle[3] + "\nI5: " + momentrectangle[4] + "\nI6: " + momentrectangle[5] + "\nI7: " + momentrectangle[6];
			String^ caption = "Learn Ellipse";
			MessageBoxButtons buttons = MessageBoxButtons::OK;
			MessageBoxIcon icon = MessageBoxIcon::Information;
			MessageBox::Show(this, text, caption, buttons, icon);
		}

		private: System::Void menustrip_edge_dxdy_Click(System::Object^  sender, System::EventArgs^  e) 
		{
			// Picture Box Refresh
			raw_intensity = BMPToIntensity(buffer, width, height);
			picturebox_first->ImageLocation = openfiledialog->FileName;
			// Dx + Dy
			/*
			derivative_h = Derivative(raw_intensity, width, height, true);
			derivative_v = Derivative(raw_intensity, width, height, false);

			int Min = 9999;
			int Max = 0;
			float* dxdy = new float[(width - 2) * (height - 2)];

			for (int i = 0; i < (width - 2) * (height - 2); i++)
				dxdy[i] = derivative_h[i] + derivative_v[i];

			for (int i = 0; i < (width - 2) * (height - 2); i++)
			{
				if (dxdy[i] > Max)
					Max = dxdy[i];

				if (dxdy[i] < Min && dxdy[i] > 0)
					Min = dxdy[i];
			}

			Min = Min / 100 * 1;
			Max = Max / 100 * 50;

			for (int i = 0; i < (width - 2) * (height - 2); i++)
			{
				if (dxdy[i] > 1)
				{
					if (Math::Abs(dxdy[i] - Min) < Math::Abs(dxdy[i] - Max))
						dxdy[i] = 0;
					else
						dxdy[i] = 255;
				}
			}

			BYTE* denemem = new BYTE[(width - 2) * (height - 2)];

			for (int i = 0; i < (width - 2) * (height - 2); i++)
			{
				denemem[i] = dxdy[i];
			}

			SurfaceSetPixel(denemem, picturebox_last, (width - 2), (height - 2));
			*/
			derivative_h = Derivative(raw_intensity, width, height, true);
			derivative_v = Derivative(raw_intensity, width, height, false);

			int Min = 99999;
			int Max = 0;
			float* dxdy = new float[(width - 2) * (height - 2)];

			for (int i = 0; i < (width - 2) * (height - 2); i++)
				dxdy[i] = derivative_h[i] + derivative_v[i];

			for (int i = 0; i < (width - 2) * (height - 2); i++)
			{
				if (dxdy[i] > Max)
					Max = dxdy[i];

				if (dxdy[i] < Min && dxdy[i] > 0)
					Min = dxdy[i];
			}

			for (int i = 0; i < (width - 2) * (height - 2); i++)
				dxdy[i] = dxdy[i] / Max;

			BYTE* denemem = new BYTE[(width - 2) * (height - 2)];

			for (int i = 0; i < (width - 2) * (height - 2); i++)
			{
				if (dxdy[i] > 0.1)
					denemem[i] = 255;
				else
					denemem[i] = 0;
			}

			SurfaceSetPixel(denemem, picturebox_last, (width - 2), (height - 2));
			
			//UI Change
			UIChange(true, true, "RGB", "Dx + Dy", false, false, false, true);
		}

		private: System::Void menustrip_edge_canny_Click(System::Object^  sender, System::EventArgs^  e) 
		{
			// Picture Box Refresh
			raw_intensity = BMPToIntensity(buffer, width, height);
			picturebox_first->ImageLocation = openfiledialog->FileName;
			// Canny
			/*
			BYTE* deneme = new BYTE[36];
			int denne[36] = { 2, 0, 5, 4, 3, 0, 0, 0, 6, 2, 7, 5, 1, 0, 2, 0, 1, 0, 6, 5, 7, 7, 6, 7, 7, 6, 7, 6, 6, 6, 6, 2, 0, 4, 3, 5 };

			for (int i = 0; i < 36; i++)
				deneme[i] = denne[i];
			
			derivative_h = Derivative(deneme, width, height, true);
			derivative_v = Derivative(deneme, width, height, false);
			arctan = Degree(derivative_h, derivative_v, (width - 2), (height - 2));
			nonmaxsupression = NonMaxSupression(derivative_h, derivative_v, arctan, (width - 2), (height - 2));
			hysteresishhreshold = HysteresisThreshold(nonmaxsupression, arctan, (width - 2), (height - 2));
			SurfaceSetPixel(hysteresishhreshold, picturebox_last, (width - 2), (height - 2));
			*/
			
			derivative_h = Derivative(raw_intensity, width, height, true);
			derivative_v = Derivative(raw_intensity, width, height, false);
			arctan = Degree(derivative_h, derivative_v, (width - 2), (height - 2));
			nonmaxsupression = NonMaxSupression(derivative_h, derivative_v, arctan, (width - 2), (height - 2));
			hysteresishhreshold = HysteresisThreshold(nonmaxsupression, arctan, (width - 2), (height - 2));
			SurfaceSetPixel(hysteresishhreshold, picturebox_last, (width - 2), (height - 2));
			
			//UI Change
			UIChange(true, true, "RGB", "Canny", false, false, false, true);
		}
	};
}

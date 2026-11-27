namespace lab_04
{
    partial class Form1
    {
        private Label sizeLabel;
        private TextBox sizeBox;
        private Button createButton;
        private Label valueLabel;
        private TextBox valueBox;
        private Button removeButton;
        private TableLayoutPanel tables;
        private GroupBox inputGroup;
        private GroupBox resultGroup;
        private DataGridView inputGrid;
        private DataGridView resultGrid;
        private Label resultLabel;

        private void InitializeComponent()
        {
            sizeLabel = new Label();
            sizeBox = new TextBox();
            createButton = new Button();
            valueLabel = new Label();
            valueBox = new TextBox();
            removeButton = new Button();
            tables = new TableLayoutPanel();
            inputGroup = new GroupBox();
            inputGrid = new DataGridView();
            resultGroup = new GroupBox();
            resultGrid = new DataGridView();
            resultLabel = new Label();
            tables.SuspendLayout();
            inputGroup.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)inputGrid).BeginInit();
            resultGroup.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)resultGrid).BeginInit();
            SuspendLayout();
            // 
            // sizeLabel
            // 
            sizeLabel.AutoSize = true;
            sizeLabel.Location = new Point(12, 17);
            sizeLabel.Name = "sizeLabel";
            sizeLabel.Size = new Size(59, 15);
            sizeLabel.TabIndex = 0;
            sizeLabel.Text = "Размер N";
            // 
            // sizeBox
            // 
            sizeBox.Location = new Point(78, 13);
            sizeBox.Name = "sizeBox";
            sizeBox.Size = new Size(48, 23);
            sizeBox.TabIndex = 0;
            // 
            // createButton
            // 
            createButton.Location = new Point(134, 12);
            createButton.Name = "createButton";
            createButton.Size = new Size(128, 25);
            createButton.TabIndex = 1;
            createButton.Text = "Создать матрицу";
            createButton.Click += create_matrix_Click;
            // 
            // valueLabel
            // 
            valueLabel.AutoSize = true;
            valueLabel.Location = new Point(281, 17);
            valueLabel.Name = "valueLabel";
            valueLabel.Size = new Size(94, 15);
            valueLabel.TabIndex = 2;
            valueLabel.Text = "Искомое число";
            // 
            // valueBox
            // 
            valueBox.Location = new Point(386, 13);
            valueBox.Name = "valueBox";
            valueBox.Size = new Size(80, 23);
            valueBox.TabIndex = 2;
            // 
            // removeButton
            // 
            removeButton.Location = new Point(474, 12);
            removeButton.Name = "removeButton";
            removeButton.Size = new Size(126, 25);
            removeButton.TabIndex = 3;
            removeButton.Text = "Удалить строки";
            removeButton.Click += remove_rows_Click;
            // 
            // tables
            // 
            tables.Anchor = AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right;
            tables.ColumnCount = 2;
            tables.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50F));
            tables.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50F));
            tables.Controls.Add(inputGroup, 0, 0);
            tables.Controls.Add(resultGroup, 1, 0);
            tables.Location = new Point(12, 48);
            tables.Name = "tables";
            tables.RowCount = 1;
            tables.RowStyles.Add(new RowStyle(SizeType.Percent, 100F));
            tables.Size = new Size(716, 359);
            tables.TabIndex = 4;
            // 
            // inputGroup
            // 
            inputGroup.Controls.Add(inputGrid);
            inputGroup.Dock = DockStyle.Fill;
            inputGroup.Location = new Point(3, 3);
            inputGroup.Name = "inputGroup";
            inputGroup.Size = new Size(352, 353);
            inputGroup.TabIndex = 0;
            inputGroup.TabStop = false;
            inputGroup.Text = "Исходная матрица";
            // 
            // inputGrid
            // 
            inputGrid.AllowUserToAddRows = false;
            inputGrid.AllowUserToDeleteRows = false;
            inputGrid.Dock = DockStyle.Fill;
            inputGrid.Location = new Point(3, 19);
            inputGrid.Name = "inputGrid";
            inputGrid.Size = new Size(346, 331);
            inputGrid.TabIndex = 0;
            // 
            // resultGroup
            // 
            resultGroup.Controls.Add(resultGrid);
            resultGroup.Dock = DockStyle.Fill;
            resultGroup.Location = new Point(361, 3);
            resultGroup.Name = "resultGroup";
            resultGroup.Size = new Size(352, 353);
            resultGroup.TabIndex = 1;
            resultGroup.TabStop = false;
            resultGroup.Text = "Результат";
            // 
            // resultGrid
            // 
            resultGrid.AllowUserToAddRows = false;
            resultGrid.AllowUserToDeleteRows = false;
            resultGrid.Dock = DockStyle.Fill;
            resultGrid.Location = new Point(3, 19);
            resultGrid.Name = "resultGrid";
            resultGrid.ReadOnly = true;
            resultGrid.Size = new Size(346, 331);
            resultGrid.TabIndex = 0;
            // 
            // resultLabel
            // 
            resultLabel.Anchor = AnchorStyles.Bottom | AnchorStyles.Left;
            resultLabel.AutoSize = true;
            resultLabel.Location = new Point(15, 417);
            resultLabel.Name = "resultLabel";
            resultLabel.Size = new Size(0, 15);
            resultLabel.TabIndex = 5;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(740, 445);
            Controls.Add(sizeLabel);
            Controls.Add(sizeBox);
            Controls.Add(createButton);
            Controls.Add(valueLabel);
            Controls.Add(valueBox);
            Controls.Add(removeButton);
            Controls.Add(tables);
            Controls.Add(resultLabel);
            MinimumSize = new Size(640, 320);
            Name = "Form1";
            StartPosition = FormStartPosition.CenterScreen;
            Text = "Лабораторная работа 4. Вариант 4";
            tables.ResumeLayout(false);
            inputGroup.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)inputGrid).EndInit();
            resultGroup.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)resultGrid).EndInit();
            ResumeLayout(false);
            PerformLayout();
        }
    }
}

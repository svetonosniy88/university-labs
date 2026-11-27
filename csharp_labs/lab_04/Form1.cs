using System.Windows.Forms;

namespace lab_04
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void create_matrix_Click(object sender, EventArgs e)
        {
            if (!int.TryParse(sizeBox.Text, out int size) || size < 1 || size > 100)
            {
                MessageBox.Show("Введите целый размер матрицы от 1 до 100.");
                sizeBox.Focus();
                return;
            }

            inputGrid.RowCount = 0;
            inputGrid.ColumnCount = size;
            inputGrid.RowCount = size;

            for (int column = 0; column < size; column++)
                inputGrid.Columns[column].HeaderText = (column + 1).ToString();

            resultGrid.RowCount = 0;
            resultGrid.ColumnCount = 0;
            resultLabel.Text = "";
            inputGrid.CurrentCell = inputGrid[0, 0];
            inputGrid.Focus();
        }

        private void remove_rows_Click(object sender, EventArgs e)
        {
            if (inputGrid.RowCount == 0)
            {
                MessageBox.Show("Сначала создайте исходную матрицу.");
                return;
            }

            if (!int.TryParse(valueBox.Text, out int value))
            {
                MessageBox.Show("Введите целое число, которое нужно искать.");
                valueBox.Focus();
                return;
            }

            inputGrid.EndEdit();
            int size = inputGrid.RowCount;
            int[,] values = new int[size, size];

            for (int row = 0; row < size; row++)
            {
                for (int column = 0; column < size; column++)
                {
                    DataGridViewCell cell = inputGrid[column, row];
                    if (!int.TryParse(cell.Value?.ToString(), out values[row, column]))
                    {
                        MessageBox.Show($"Введите целое число. Строка {row + 1}, столбец {column + 1}.");
                        inputGrid.CurrentCell = cell;
                        inputGrid.Focus();
                        return;
                    }
                }
            }

            Matrix matrix = new Matrix(values);
            int[,] result = matrix.remove_rows(value);
            int rows = result.GetLength(0);
            int columns = result.GetLength(1);

            resultGrid.RowCount = 0;
            resultGrid.ColumnCount = columns;
            resultGrid.RowCount = rows;

            for (int column = 0; column < columns; column++)
                resultGrid.Columns[column].HeaderText = (column + 1).ToString();

            for (int row = 0; row < rows; row++)
            {
                for (int column = 0; column < columns; column++)
                    resultGrid[column, row].Value = result[row, column];
            }

            if (rows == 0)
                resultLabel.Text = "Удалены все строки.";
            else if (rows == size)
                resultLabel.Text = "Совпадений нет.";
            else
                resultLabel.Text = $"Осталось строк: {rows} из {size}.";
        }
    }
}

namespace lab_04
{
    internal class Matrix
    {
        private readonly int[,] values;

        public Matrix(int[,] values)
        {
            this.values = values;
        }

        public int[,] remove_rows(int value)
        {
            int rowCount = values.GetLength(0);
            int columnCount = values.GetLength(1);
            bool[] remove = new bool[rowCount];
            int remaining = rowCount;

            for (int row = 0; row < rowCount; row++)
            {
                for (int column = 0; column < columnCount; column++)
                {
                    if (values[row, column] == value)
                    {
                        remove[row] = true;
                        remaining--;
                        break;
                    }
                }
            }

            int[,] result = new int[remaining, columnCount];
            int resultRow = 0;

            for (int row = 0; row < rowCount; row++)
            {
                if (remove[row])
                    continue;

                for (int column = 0; column < columnCount; column++)
                    result[resultRow, column] = values[row, column];

                resultRow++;
            }

            return result;
        }
    }
}

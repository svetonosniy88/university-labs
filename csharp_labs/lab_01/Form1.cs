namespace lab_01
{
    public partial class Form1 : Form
    {
        private const string InitialTitle = "Лабораторная работа № 1 — координаты мыши";

        public Form1()
        {
            InitializeComponent();
            Text = InitialTitle;
            MouseClick += Form1_MouseClick;
        }

        private void Form1_MouseClick(object? sender, MouseEventArgs e)
        {
            Text = $"Координаты мыши: X = {e.Location.X}, Y = {e.Location.Y}";
        }
    }
}

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WysHost
{
    public partial class UpdateAccount : Form
    {
        public UpdateAccount()
        {
            InitializeComponent();

            showPassBtn.BackgroundImage = Image.FromFile("../../Images/show.jpg");
            showPassBtn.BackgroundImageLayout = ImageLayout.Stretch;
            passwordTextBox.UseSystemPasswordChar = true;
        }

        private void submitBtn_Click(object sender, EventArgs e)
        {
            MessageBox.Show("TODO");
        }

        private void clearBtn_Click(object sender, EventArgs e) => accountBindingSource.Clear();

        private void retBtn_Click(object sender, EventArgs e) => Close();

        private void showPassBtn_Click(object sender, EventArgs e)
        {
            if (passwordTextBox.UseSystemPasswordChar)
                showPassBtn.BackgroundImage = Image.FromFile("../../Images/hide.jpg");
            else
                showPassBtn.BackgroundImage = Image.FromFile("../../Images/show.jpg");

            passwordTextBox.UseSystemPasswordChar = !passwordTextBox.UseSystemPasswordChar;
        }
    }
}

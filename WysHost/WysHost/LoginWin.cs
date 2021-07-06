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
    public partial class LoginWin : Form
    {
        public LoginWin()
        {
            InitializeComponent();
        }

        private void retBtn_Click(object sender, EventArgs e) => Close();

        private void loginBtn_Click(object sender, EventArgs e)
        {
            MessageBox.Show("TODO: send data to DAL for encryption and than send to server");
        }
    }
}

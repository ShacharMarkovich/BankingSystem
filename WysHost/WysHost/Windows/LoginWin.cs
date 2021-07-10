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
            passwordTextBox.PasswordChar = '*';
        }

        private void retBtn_Click(object sender, EventArgs e) => Close();

        private void loginBtn_Click(object sender, EventArgs e)
        {
            if (usernameTextBox.Text != "" && passwordTextBox.Text != "")
                MessageBox.Show("TODO: send data to DAL for encryption\nTODO: than send to server");
                
            else
                MessageBox.Show("Please fill both username & password!");
            new AccountPage().ShowDialog();
        }

        private void testBtn_Click(object sender, EventArgs e)
        {
            Connector c = Connector.getInstance();
        }
    }
}

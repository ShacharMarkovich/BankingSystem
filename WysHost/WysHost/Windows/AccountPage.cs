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
    public partial class AccountPage : Form
    {
        public AccountPage()
        {
            InitializeComponent();
        }

        private void updateBtn_Click(object sender, EventArgs e)
        {
            new UpdateAccount().ShowDialog();
        }

        private void logoutBtn_Click(object sender, EventArgs e)
        {
            MessageBox.Show("TODO: logout");
            Close();
        }
    }
}

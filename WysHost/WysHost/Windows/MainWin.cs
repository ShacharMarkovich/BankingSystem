using System;
using System.Windows.Forms;

namespace WysHost
{
    public partial class MainWin : Form
    {
        public MainWin() { InitializeComponent(); }

        private void loginBtm_Click(object sender, EventArgs e) => new LoginWin().ShowDialog();

        private void registerBtm_Click(object sender, EventArgs e) => new RegisterWin().ShowDialog();
    }
}

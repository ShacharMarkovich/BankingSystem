using System;
using System.Windows.Forms;

namespace WysHost
{
    public partial class AccountPage : Form
    {
        Account _currAccount;
        public AccountPage(Account acc)
        {
            InitializeComponent();
            _currAccount = acc;
        }

        private void updateBtn_Click(object sender, EventArgs e) => new UpdateAccount(_currAccount).ShowDialog();

        private void logoutBtn_Click(object sender, EventArgs e)
        {
            MessageBox.Show("TODO: logout");
            Close();
        }
    }
}

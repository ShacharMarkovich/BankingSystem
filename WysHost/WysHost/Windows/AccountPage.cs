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
            balanceLbl.Text = "Balance: " + _currAccount.Balance.ToString();
            accNumLbl.Text = "Account Number: " + _currAccount.accNum.ToString();
        }

        private void updateBtn_Click(object sender, EventArgs e) => new UpdateAccount(_currAccount).ShowDialog();

        private void logoutBtn_Click(object sender, EventArgs e)
        {
            MainWin.connector.SendAndRecvServer(Client.serverOpcode.logout, "");
            Close();
        }
    }
}

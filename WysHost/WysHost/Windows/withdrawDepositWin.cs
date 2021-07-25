using Newtonsoft.Json;
using System;
using System.Windows.Forms;

namespace WysHost.Windows
{
    public partial class withdrawDepositWin : Form
    {
        Account _currAccount;
        bool _isWithdrawState;
        public withdrawDepositWin(Account acc, bool isWithdrawState)
        {
            InitializeComponent();
            _currAccount = acc;
            _isWithdrawState = isWithdrawState;
            if (_isWithdrawState)
                this.Text = "Withdraw Window";
            else
                this.Text = "Deposit Window";
        }

        private void submitBtn_Click(object sender, EventArgs e)
        {
            int amount;
            if (int.TryParse(_amountInput.Text, out amount))
            {
                var loginData = new { amount = _isWithdrawState ? -amount : amount };
                string jsonString = JsonConvert.SerializeObject(loginData);
                string res = MainWin.connector.SendAndRecvServer(_isWithdrawState ? Client.serverOpcode.withdraw : Client.serverOpcode.deposit, jsonString);
            }
            else
                MessageBox.Show("please enter a number!");
        }
    }
}

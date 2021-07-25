using Client;
using Newtonsoft.Json;
using System;
using System.Windows.Forms;

namespace WysHost.Windows
{
    public partial class withdrawDepositWin : Form
    {
        bool _isWithdrawState;
        public withdrawDepositWin(bool isWithdrawState)
        {
            InitializeComponent();
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
                if (amount > 0)
                {
                    var loginData = new { amount = _isWithdrawState ? -amount : amount };
                    string jsonString = JsonConvert.SerializeObject(loginData);
                    string res = MainWin.connector.SendAndRecvServer(_isWithdrawState ? serverOpcode.withdraw : serverOpcode.deposit, jsonString);
                    if (res[0] == Utils.SUCCESSED)
                        AccountPage._currAccount.Balance += _isWithdrawState ? -amount : amount;
                    MessageBox.Show(res.Substring(2));
                }
                else MessageBox.Show("Do not be a wiseguy.");
            }
            else MessageBox.Show("please enter a number!");
        }

        private void cancelBtn_Click(object sender, EventArgs e) => Close();
    }
}

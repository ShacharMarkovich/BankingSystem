namespace WysHost
{
    partial class AccountPage
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.balanceLbl = new System.Windows.Forms.Label();
            this.updateBtn = new System.Windows.Forms.Button();
            this.withdrawBtn = new System.Windows.Forms.Button();
            this.depositBtn = new System.Windows.Forms.Button();
            this.transferBtn = new System.Windows.Forms.Button();
            this.logoutBtn = new System.Windows.Forms.Button();
            this.accNumLbl = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // balanceLbl
            // 
            this.balanceLbl.AutoSize = true;
            this.balanceLbl.Location = new System.Drawing.Point(13, 13);
            this.balanceLbl.Name = "balanceLbl";
            this.balanceLbl.Size = new System.Drawing.Size(59, 13);
            this.balanceLbl.TabIndex = 0;
            this.balanceLbl.Text = "Balance: X";
            // 
            // updateBtn
            // 
            this.updateBtn.Location = new System.Drawing.Point(16, 47);
            this.updateBtn.Name = "updateBtn";
            this.updateBtn.Size = new System.Drawing.Size(142, 23);
            this.updateBtn.TabIndex = 1;
            this.updateBtn.Text = "Update Acount Data";
            this.updateBtn.UseVisualStyleBackColor = true;
            this.updateBtn.Click += new System.EventHandler(this.updateBtn_Click);
            // 
            // withdrawBtn
            // 
            this.withdrawBtn.Location = new System.Drawing.Point(16, 94);
            this.withdrawBtn.Name = "withdrawBtn";
            this.withdrawBtn.Size = new System.Drawing.Size(142, 23);
            this.withdrawBtn.TabIndex = 2;
            this.withdrawBtn.Text = "Withdraw Money";
            this.withdrawBtn.UseVisualStyleBackColor = true;
            // 
            // depositBtn
            // 
            this.depositBtn.Location = new System.Drawing.Point(16, 144);
            this.depositBtn.Name = "depositBtn";
            this.depositBtn.Size = new System.Drawing.Size(142, 23);
            this.depositBtn.TabIndex = 3;
            this.depositBtn.Text = "Deposit Money";
            this.depositBtn.UseVisualStyleBackColor = true;
            // 
            // transferBtn
            // 
            this.transferBtn.Location = new System.Drawing.Point(16, 197);
            this.transferBtn.Name = "transferBtn";
            this.transferBtn.Size = new System.Drawing.Size(142, 23);
            this.transferBtn.TabIndex = 4;
            this.transferBtn.Text = "Transfer Money";
            this.transferBtn.UseVisualStyleBackColor = true;
            // 
            // logoutBtn
            // 
            this.logoutBtn.Location = new System.Drawing.Point(259, 197);
            this.logoutBtn.Name = "logoutBtn";
            this.logoutBtn.Size = new System.Drawing.Size(75, 23);
            this.logoutBtn.TabIndex = 5;
            this.logoutBtn.Text = "Logout";
            this.logoutBtn.UseVisualStyleBackColor = true;
            this.logoutBtn.Click += new System.EventHandler(this.logoutBtn_Click);
            // 
            // accNumLbl
            // 
            this.accNumLbl.AutoSize = true;
            this.accNumLbl.Location = new System.Drawing.Point(188, 13);
            this.accNumLbl.Name = "accNumLbl";
            this.accNumLbl.Size = new System.Drawing.Size(100, 13);
            this.accNumLbl.TabIndex = 6;
            this.accNumLbl.Text = "Account Number: Y";
            // 
            // AccountPage
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(346, 245);
            this.Controls.Add(this.accNumLbl);
            this.Controls.Add(this.logoutBtn);
            this.Controls.Add(this.transferBtn);
            this.Controls.Add(this.depositBtn);
            this.Controls.Add(this.withdrawBtn);
            this.Controls.Add(this.updateBtn);
            this.Controls.Add(this.balanceLbl);
            this.Name = "AccountPage";
            this.Text = "Welcom Back NAME";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label balanceLbl;
        private System.Windows.Forms.Button updateBtn;
        private System.Windows.Forms.Button withdrawBtn;
        private System.Windows.Forms.Button depositBtn;
        private System.Windows.Forms.Button transferBtn;
        private System.Windows.Forms.Button logoutBtn;
        private System.Windows.Forms.Label accNumLbl;
    }
}
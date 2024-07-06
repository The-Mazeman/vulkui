opt = {noremap = true, silent = true}
vim.keymap.set("n", "<leader>e", vim.cmd.Ex, opt)
vim.keymap.set("n", "<leader>h", ":bp<CR>", opt)
vim.keymap.set("n", "<leader>l", ":bn<CR>", opt)
vim.keymap.set("n", "<leader>q", ":bd<CR>", opt)

vim.keymap.set("n", "<C-d>", "<C-d>zz")
vim.keymap.set("n", "<C-u>", "<C-u>zz")

vim.keymap.set("n", "n", "nzzzv")
vim.keymap.set("n", "N", "Nzzzv")

vim.keymap.set("n", "<C-h>", "<C-w>h", opts)
vim.keymap.set("n", "<C-j>", "<C-w>j", opts)
vim.keymap.set("n", "<C-k>", "<C-w>k", opts)
vim.keymap.set("n", "<C-l>", "<C-w>l", opts)

vim.keymap.set("n", "<leader>p", 'viw"ap', opts)
vim.keymap.set("n", "<leader>x", '"_x', opts)
vim.keymap.set("n", "<leader>y", 'viw"ay', opts)

vim.keymap.set("n", "<leader>n", ':lua require("harpoon.ui").toggle_quick_menu()<CR>', opts)
vim.keymap.set("n", "<leader>m", ':lua require("harpoon.mark").add_file()<CR>', opts)
vim.keymap.set("n", "<leader>1", ':lua require("harpoon.ui").nav_file(1)<CR>', opts)
vim.keymap.set("n", "<leader>2", ':lua require("harpoon.ui").nav_file(2)<CR>', opts)
vim.keymap.set("n", "<leader>3", ':lua require("harpoon.ui").nav_file(3)<CR>', opts)
vim.keymap.set("n", "<leader>4", ':lua require("harpoon.ui").nav_file(4)<CR>', opts)
vim.keymap.set("n", "<leader>5", ':lua require("harpoon.ui").nav_file(5)<CR>', opts)
vim.keymap.set("n", "<leader>5", ':lua require("harpoon.ui").nav_file(5)<CR>', opts)
vim.keymap.set("n", "<leader>6", ':lua require("harpoon.ui").nav_file(6)<CR>', opts)
vim.keymap.set("n", "<leader>7", ':lua require("harpoon.ui").nav_file(7)<CR>', opts)

vim.keymap.set("i", "<C-k>", '<C-n>', opts)
vim.keymap.set("i", "<C-i>", '<C-p>', opts)

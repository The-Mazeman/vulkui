require("custom.initLazy")
require("custom.remap")
require("custom.lualine")

vim.opt.guicursor = "a:block"
vim.opt.laststatus = 3
vim.opt.mouse = ""
vim.opt.showmode = false
vim.opt.nu = false
vim.opt.relativenumber = false

vim.opt.tabstop = 4
vim.opt.softtabstop = 4
vim.opt.shiftwidth = 4
vim.opt.expandtab = true
vim.opt.fillchars = "vert:|,horiz:-"

vim.opt.smartindent = true
vim.opt.wrap = false
vim.opt.swapfile = false
vim.opt.backup = false
vim.opt.hlsearch = false
vim.opt.incsearch = true
vim.opt.termguicolors = true
vim.opt.scrolloff = 8

vim.cmd("syntax off")

require('lualine').setup {
  options = {
    icons_enabled = false,
    theme = 'gruvbox',
    component_separators = {left = "", right = ""},
    section_separators = {left = "", right = ""},
    disabled_filetypes = {
      statusline = {},
      winbar = {},
    },
    ignore_focus = {},
    always_divide_middle = false,
    globalstatus = false,
    refresh = {
      statusline = 1000,
      tabline = 1000,
      winbar = 1000,
    }
  },
  sections = 
  {
    lualine_a = {'buffers'},
    lualine_b = {''},
    lualine_c = {''},
    lualine_x = {''},
    lualine_y = {''},
    lualine_z = {''},
  },
  
  inactive_sections = 
  {
    lualine_a = {''},
    lualine_b = {''},
    lualine_c = {''},
    lualine_x = {''},
    lualine_y = {''},
    lualine_z = {''},
  },
  tabline = {},
  winbar = 
  {
    lualine_a = {''},
    lualine_b = {''},
    lualine_c = {''},
    lualine_x = {''},
    lualine_y = {''},
    lualine_z = {''},
  },
  inactive_winbar = {},
  extensions = {}
}

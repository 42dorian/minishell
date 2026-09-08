/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guthybarnakoppany <guthybarnakoppany@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 19:02:10 by bguhty            #+#    #+#             */
/*   Updated: 2026/09/08 14:37:33 by guthybarnak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env_assign_helpers.c"
#include "rest_helpers.c"
#include "split_helpers.c"
#include "split.c"
#include "stepping_in_input.c"
#include "syntax_error_check.c"
#include "expansion_check.c"
#include "environment_creation.c"
#include "skippers.c"
#include "tokenizing.c"
#include "dollar_sign_handler.c"
#include "is_special_character.c"
#include "special_characters_checkers.c"
#include "word_count_helpers.c"
#include "libft/ft_strlen.c"
#include "libft/list_general.c"
#include "libft/list_helpers.c"
#include "libft/ft_calloc.c"
#include "libft/ft_memset.c"
#include "libft/ft_bzero.c"
#include "libft/ft_itoa.c"

volatile sig_atomic_t g_signal = 0;

int     determine_quote_type(char letter, int quote_type)
{
    if (quote_type == SINGLE_QUOTE && letter == SINGLE_QUOTE)
        quote_type = 0;
    else if (quote_type == DOUBLE_QUOTE && letter == DOUBLE_QUOTE)
        quote_type = 0;
    else if (quote_type == 0 && letter == SINGLE_QUOTE)
        quote_type = SINGLE_QUOTE;
    else if (quote_type == 0 && letter == DOUBLE_QUOTE)
        quote_type = DOUBLE_QUOTE;
    return (quote_type);
}

int     count_valid_char(const char *quoted_word)
{
    int i;
    int counter;
    int quote_type;

    quote_type = 0;
    counter = 0;
    i = 0;
    while (quoted_word[i])
    {
        get_real_quote_type(quoted_word, &quote_type, &i);
        if (quoted_word[i] != quote_type)
        {
            counter++;
            i++;
        }
    }
    return (counter);
}

void    get_real_quote_type(const char *word, int *quote_type, int *i)
{
    while (check_for_quote(word[*i], quote_type) && word[*i] == word[(*i) + 1])
        (*i)++;
    if (!check_for_quote(word[*i], quote_type))
        return ;
    if (*quote_type != 0)
    {
        if ((*quote_type = determine_quote_type(word[*i], *quote_type)) == 0)
            (*i)++;
    }
    else
    {
        *quote_type = determine_quote_type(word[*i], *quote_type);
        (*i)++;
    }
}

int    malloc_for_new_word(char **new_word, int len)
{
    *new_word = malloc(sizeof(char) * (len + 1));
    if (!new_word)
        return (0);
    else
        return (1);
}

char    *get_rid_of_them_quotes(t_token *tokens, int i)
{
    int     j;
    int     quote_type;
    int     local_index;
    char    *new_word;

    quote_type = 0;
    j = 0;
    local_index = 0;
    if (!malloc_for_new_word(&new_word, count_valid_char(tokens[i].value)))
        return (free((void*)tokens[i].value), NULL);
    while (tokens[i].value[j])
    {
        if (check_for_quote(tokens[i].value[j], &quote_type))
        {
            j++;
            while (tokens[i].value[j] != quote_type && tokens[i].value[j])
                new_word[local_index++] = tokens[i].value[j++];
            j++;
        }
        else if (tokens[i].value[j])
            new_word[local_index++] = tokens[i].value[j++];
    }
    new_word[local_index] = 0;
    free((void*)tokens[i].value);
    return (new_word);
}

int    remove_quotes(t_token *tokens)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while (tokens[i].value)
    {
        while (tokens[i].value[j])
        {
            if (is_quote(tokens[i].value[j]))
            {
                tokens[i].value = get_rid_of_them_quotes(tokens, i);
                if (!tokens[i].value)
                    return (0);
                break ;
            }
            j++;
        }
        i++;
        j = 0;
    }
    return (1);
}

void    copy_key(const char *envp, char *new_key)
{
    int i;

    i = 0;
    while (envp[i] != EQUAL_SIGN)
    {
        new_key[i] = envp[i];
        i++;
    }
    new_key[i] = 0;
}

char    *insert_key(const char *envp)
{
    char    *new_key;

    new_key = malloc(sizeof(char) * (key_counter(envp) + 1));
    if (!new_key)
        return (NULL);
    copy_key(envp, new_key);
    return (new_key);
}

void    copy_value(const char *envp, char *new_value)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while (envp[i] != EQUAL_SIGN)
        i++;
    i++;
    while (envp[i])
        new_value[j++] = envp[i++];
    new_value[j] = 0;
}

char    *insert_value(const char *envp)
{
    char    *new_value;

    new_value = malloc(sizeof(char) * (value_counter(envp) + 1));
    if (!new_value)
        return (NULL);
    copy_value(envp, new_value);
    return (new_value);
}

int     clean_up_env_list(t_envs **env_list)
{
    ft_lstclear(env_list, free);
    return (0);
}

int     get_env_len(const char **envp)
{
    int len;

    len = 0;
    while (envp[len])
        len++;
    return (len);
}

int    add_envp_to_list(t_envs **my_list, const char **envp)
{
    t_envs *new_node;
    int     i;
    int     envp_len;

    i = 0;
    envp_len = get_env_len(envp);
    while (i < envp_len)
    {
        new_node = copy_from_envp_to_own_env_list(envp, i);
        if (!new_node)
            return(clean_up_env_list(my_list));
        else
            ft_lstadd_back(my_list, new_node);
        i++;
    }
    return (1);
}

t_envs     *copy_from_envp_to_own_env_list(const char **envp, int i)
{
    t_envs *new_node;

    new_node = malloc(sizeof(t_envs));
    if (!new_node)
        return (NULL);
    new_node->key = insert_key(envp[i]);
    if (!new_node->key)
        return (free(new_node), NULL);
    new_node->value = insert_value(envp[i]);
    if (!new_node->value)
        return (free(new_node->key), free(new_node), NULL);
    new_node->next = NULL;
    return (new_node);
}

void        clean_up_tokens_and_split_line(t_token *tokens, char **split_line)
{
    int i;
    
    i = 0;
    while (tokens[i].type != -1)
        free((void*)tokens[i++].value);
    i = 0;
    while (split_line[i])
        free(split_line[i++]);
    free(split_line);
    free(tokens);
}     

t_token     *minishell(const char *read_line, t_envs *env_list, int *status)
{
    int     i;
    t_token *tokens;
    char    **split_line;

    i = 0;
    if(*read_line == '\0')
   		return (NULL);
    split_line = split_read_line(read_line);
    if (!split_line)
        return (NULL);
    tokens = malloc(sizeof(t_token) * (word_counter(read_line) + 1));
    if (!tokens)
        return (split_clean_up(split_line, word_counter(read_line)), NULL);
    if (!create_token_struct(tokens, split_line))
        return (clean_up_tokens_and_split_line(tokens, split_line), NULL);
    //printf("GEC2\n");
    split_clean_up(split_line, word_counter(read_line));
    //printf("GEC3\n");
    printf("before expansion: %s\n", tokens[i].value);
    if (!handle_expansions(env_list, tokens, status))
        return (NULL);
    //printf("GEC4\n");
    printf("after expansion: %s\n", tokens[i].value);
    if (!remove_quotes(tokens))
        return (clean_up_token_and_env_list(tokens, &env_list), NULL);
    printf("after expansion and removing quotes: %s\n", tokens[i].value);
    syntax_check(tokens, status);
    return (tokens);
}

int main()//int args, char **argv, char **envp)
{
	char *line;
	int status;
	t_token *tokens;
	t_envs *global_envs;
	t_cmds *cmds;

    line = malloc(sizeof(char) * (9));
    line[0] = '"';
    //line[1] = '\'';
    line[1] = '$';
    line[2] = 'S';
    line[3] = 'H';
    line[4] = 'E';
    line[5] = 'L';
    line[6] = 'L';
    //line[8] = '\'';
    line[7] = '"';
    line[8] = 0;
    char *envp[] ={"SHELL=/bin/bash",
    "COLORTERM=truecolor",
    "TERM_PROGRAM_VERSION=1.107.0",
    "SSH_AUTH_SOCK=/run/user/501/vscode-ssh-auth-sock-711369456",
    "HOMEBREW_PREFIX=/home/linuxbrew/.linuxbrew",
    "PWD=/home/guthybarnakoppany/minishell",
    "LOGNAME=guthybarnakoppany",
    "XDG_SESSION_TYPE=tty",
    "VSCODE_GIT_ASKPASS_NODE=/home/guthybarnakoppany/.vscode-server/cli/servers/Stable-618725e67565b290ba4da6fe2d29f8fa1d4e3622/server/node",
    "HOME=/home/guthybarnakoppany",
    "LANG=en_US.UTF-8",
    "LS_COLORS=rs=0:di=01;34:ln=01;36:mh=00:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:mi=00:su=37;41:sg=30;43:ca=00:tw=30;42:ow=34;42:st=37;44:ex=01;32:*.tar=01;31:*.tgz=01;31:*.arc=01;31:*.arj=01;31:*.taz=01;31:*.lha=01;31:*.lz4=01;31:*.lzh=01;31:*.lzma=01;31:*.tlz=01;31:*.txz=01;31:*.tzo=01;31:*.t7z=01;31:*.zip=01;31:*.z=01;31:*.dz=01;31:*.gz=01;31:*.lrz=01;31:*.lz=01;31:*.lzo=01;31:*.xz=01;31:*.zst=01;31:*.tzst=01;31:*.bz2=01;31:*.bz=01;31:*.tbz=01;31:*.tbz2=01;31:*.tz=01;31:*.deb=01;31:*.rpm=01;31:*.jar=01;31:*.war=01;31:*.ear=01;31:*.sar=01;31:*.rar=01;31:*.alz=01;31:*.ace=01;31:*.zoo=01;31:*.cpio=01;31:*.7z=01;31:*.rz=01;31:*.cab=01;31:*.wim=01;31:*.swm=01;31:*.dwm=01;31:*.esd=01;31:*.avif=01;35:*.jpg=01;35:*.jpeg=01;35:*.mjpg=01;35:*.mjpeg=01;35:*.gif=01;35:*.bmp=01;35:*.pbm=01;35:*.pgm=01;35:*.ppm=01;35:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.tiff=01;35:*.png=01;35:*.svg=01;35:*.svgz=01;35:*.mng=01;35:*.pcx=01;35:*.mov=01;35:*.mpg=01;35:*.mpeg=01;35:*.m2v=01;35:*.mkv=01;35:*.webm=01;35:*.webp=01;35:*.ogm=01;35:*.mp4=01;35:*.m4v=01;35:*.mp4v=01;35:*.vob=01;35:*.qt=01;35:*.nuv=01;35:*.wmv=01;35:*.asf=01;35:*.rm=01;35:*.rmvb=01;35:*.flc=01;35:*.avi=01;35:*.fli=01;35:*.flv=01;35:*.gl=01;35:*.dl=01;35:*.xcf=01;35:*.xwd=01;35:*.yuv=01;35:*.cgm=01;35:*.emf=01;35:*.ogv=01;35:*.ogx=01;35:*.aac=00;36:*.au=00;36:*.flac=00;36:*.m4a=00;36:*.mid=00;36:*.midi=00;36:*.mka=00;36:*.mp3=00;36:*.mpc=00;36:*.ogg=00;36:*.ra=00;36:*.wav=00;36:*.oga=00;36:*.opus=00;36:*.spx=00;36:*.xspf=00;36:*~=00;90:*#=00;90:*.bak=00;90:*.old=00;90:*.orig=00;90:*.part=00;90:*.rej=00;90:*.swp=00;90:*.tmp=00;90:*.dpkg-dist=00;90:*.dpkg-old=00;90:*.ucf-dist=00;90:*.ucf-new=00;90:*.ucf-old=00;90:*.rpmnew=00;90:*.rpmorig=00;90:*.rpmsave=00;90:",
    "SSL_CERT_DIR=/usr/lib/ssl/certs",
    "GIT_ASKPASS=/home/guthybarnakoppany/.vscode-server/cli/servers/Stable-618725e67565b290ba4da6fe2d29f8fa1d4e3622/server/extensions/git/dist/askpass.sh",
    "SSH_CONNECTION=::1 0 ::1 22",
    "INFOPATH=/home/linuxbrew/.linuxbrew/share/info:",
    "VSCODE_GIT_ASKPASS_EXTRA_ARGS=",
    "VSCODE_PYTHON_AUTOACTIVATE_GUARD=1",
    "LESSCLOSE=/usr/bin/lesspipe %s %s",
    "XDG_SESSION_CLASS=user",
    "TERM=xterm-256color",
    "feher=fasz",
    "LESSOPEN=| /usr/bin/lesspipe %s",
    "USER=guthybarnakoppany",
    "VSCODE_GIT_IPC_HANDLE=/run/user/501/vscode-git-4fa45dc6e4.sock",
    "HOMEBREW_CELLAR=/home/linuxbrew/.linuxbrew/Cellar",
    "SHLVL=2",
    "HOMEBREW_REPOSITORY=/home/linuxbrew/.linuxbrew/Homebrew",
    "XDG_RUNTIME_DIR=/run/user/501",
    "SSL_CERT_FILE=/usr/lib/ssl/cert.pem",
    "DEBUGINFOD_URLS=https://debuginfod.ubuntu.com ",
    "VSCODE_GIT_ASKPASS_MAIN=/home/guthybarnakoppany/.vscode-server/cli/servers/Stable-618725e67565b290ba4da6fe2d29f8fa1d4e3622/server/extensions/git/dist/askpass-main.js",
    "BROWSER=/home/guthybarnakoppany/.vscode-server/cli/servers/Stable-618725e67565b290ba4da6fe2d29f8fa1d4e3622/server/bin/helpers/browser.sh",
    "PATH=/home/guthybarnakoppany/.local/funcheck/host:/home/linuxbrew/.linuxbrew/bin:/home/linuxbrew/.linuxbrew/sbin:/home/guthybarnakoppany/.vscode-server/cli/servers/Stable-618725e67565b290ba4da6fe2d29f8fa1d4e3622/server/bin/remote-cli:/home/guthybarnakoppany/.local/bin:/home/guthybarnakoppany/.local/bin:/opt/orbstack-guest/bin-hiprio:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/opt/orbstack-guest/bin:/opt/orbstack-guest/data/bin/cmdlinks:/home/guthybarnakoppany/.local/bin:/home/guthybarnakoppany/.local/bin:/home/guthybarnakoppany/.local/bin:/home/guthybarnakoppany/.local/bin",
    "DBUS_SESSION_BUS_ADDRESS=unix:path=/run/user/501/bus",
    "TERM_PROGRAM=vscode",
    "VSCODE_IPC_HOOK_CLI=/run/user/501/vscode-ipc-3143531e-3d1c-480d-ae4c-a7c1c2fdd299.sock",
    "_=/usr/bin/env", NULL};
	cmds = NULL;
	global_envs = NULL;
	status = 0;
	if (!add_envp_to_list(&global_envs, (const char **)envp))
        return (1);
	// init_interactive_signals();
	// while ((line = readline("minishell$ ")))
	// {
	// 	if (!ft_strncmp(line, "exitcode", 8))
	// 	{
	// 		printf("%d\n", status);
	// 		continue;
	// 	}
	// 	tokens = minishell(line, global_envs, &status);
	// 	if (!tokens)
	// 		continue;
	// 	cmds = build_cmds(tokens);
	// 	if (!cmds)
	// 	{
	// 		status = 1;
	// 		continue;
	// 	}
	// 	status = execute_cmds(cmds, envp);
	// 	if (line[0] != '\0' || !line)
	// 		add_history(line);
	// }
    tokens = minishell(line, global_envs, &status);
    if (tokens)
        clean_up_token_and_env_list(tokens, &global_envs);
    free(line);
    return (0);
}

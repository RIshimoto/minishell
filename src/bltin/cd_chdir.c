#include "../../includes/minishell.h"

static void	set_pwd(char *new_path, t_shell *shell)
{
	char	*param;
	char	*set_path;
	char	*pwd;

	pwd = getenv("PWD");
	if (pwd == NULL || ft_strcmp(pwd, "") == 0)
		param = ft_strjoin("OLDPWD=", "");
	else
		param = ft_strjoin("OLDPWD=", shell->pwd);
	set_shell_var(shell, param);
	free(param);
	set_path = ft_strdup(new_path);
	param = ft_strjoin("PWD=", new_path);
	set_shell_var(shell, param);
	free(param);
	free(shell->pwd);
	shell->pwd = set_path;
}

static char	*get_cwd_path(void)
{
	char	*cwd_path;

	cwd_path = getcwd(NULL, 0);
	if (!cwd_path)
		ft_putendl_fd("cd: error retrieving current directory: \
getcwd: cannot access parent directories: No such file or directory", 2);
	return (cwd_path);
}

static void	set_pwd_process(char *absolute_path, bool is_absolute_path, \
bool is_first_chdir, t_shell *shell)
{
	char	*cwd_path;
	char	*change_path;

	cwd_path = get_cwd_path();
	if (is_first_chdir)
	{
		change_path = absolute_path;
		if (is_absolute_path == false)
			change_path = cwd_path;
	}
	else
	{
		change_path = cwd_path;
		if (change_path == NULL)
			change_path = absolute_path;
	}
	set_pwd(change_path, shell);
	free(absolute_path);
	free(cwd_path);
}

int	change_dir(char *path, t_shell *shell)
{
	char	*absolute_path;
	char	*change_path;
	bool	is_absolute_path;

	change_path = NULL;
	absolute_path = get_absolute_path(path, &is_absolute_path, shell);
	if (chdir(absolute_path) == 0)
	{
		set_pwd_process(absolute_path, is_absolute_path, true, shell);
		return (0);
	}
	if (chdir(path) == 0)
	{
		set_pwd_process(absolute_path, is_absolute_path, false, shell);
		return (0);
	}
	free(absolute_path);
	return (1);
}

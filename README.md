# ClearV2
We are so back

# Grammar
- Each production has its own function that will return an optional typed for the item on the left

- ** Production rules formatting available when previewing mdarkdown in VSCode

### Production rules:

$$

\begin{align}

[\text{Prog}] &\to [\text{Stmt}]^*
\\
[\text{Stmt}] &\to 
\begin{cases}
\text{exit}([\text{Expr}]); \\
\text{let}\space\text{ident} = [\text{Expr}];
\end{cases}
\\
[\text{Expr}] &\to 
\begin{cases}
[\text{Term}] \\
[\text{BinExpr}]
\end{cases}
\\
[\text{BinExpr}] &\to 
\begin{cases}
[\text{Expr}] + [\text{Expr}] \space &prec=0\\
[\text{Expr}] - [\text{Expr}] \space &prec=0\\
[\text{Expr}] * [\text{Expr}] \space &prec=1\\ 
[\text{Expr}]\space / \space[\text{Expr}] \space &prec=1\\
\end{cases}
\\
[\text{Term}] &\to
\begin{cases}
\text{int\_lit} \\
\text{ident}
\end{cases}

\end{align}

$$
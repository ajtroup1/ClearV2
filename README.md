# ClearV2
We are so back

# Grammar
- Each production has its own function that will return an optional typed for the item on the left

- ** Production rules formatting available when previewing mdarkdown in VSCode

### Production rules:

$$

\begin{align}

[\text{prog}] &\to [\text{stmt}]^*
\\
[\text{stmt}] &\to 
\begin{cases}
exit([\text{expr}]); \\
let\space\text{ident} = [\text{expr}];
\end{cases}
\\
[\text{expr}] &\to \text{int\_lit}
\\


\end{align}

$$
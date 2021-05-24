# ykcbf

A collection of simple BF interpreters:

  * `bf_base.c`: the base BF interpreter upon which others are built. This
    deliberately does not do any preprocessing/optimisation of the BF program.
  * `yk_simple_yk.c`: the minimum changes from `bf_base.c` to make the BF
    interpreter run under yk.
  * `yk_simple2_yk.c`: adds some simple trace optimisations to
    `yk_simple_yk.c`.

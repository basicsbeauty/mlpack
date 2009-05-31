binrule(
    name = "empirical_markov",
    sources = ["empirical_markov.cc"],
    headers = ["empirical_mmk.h",
               "empirical_mmk_impl.h",
               "utils.h"],
    deplibs = ["fastlib:fastlib",
               "mlpack/kde:dualtree_kde",
               "mlpack/series_expansion:series_expansion",
               "contrib/niche/kmeans_nonempty:kmeans_nonempty"]
    )

binrule(
    name = "test_dna_bayes",
    sources = ["test_dna_bayes.cc"],
    headers = ["test_engine.h",
               "test_engine_impl.h",
               "utils.h",
               "multinomial.h",               
               "isotropic_gaussian.h",
               "diag_gaussian.h",
               "mixture.h",
               "hmm.h"],
    deplibs = ["fastlib:fastlib",
               "mlpack/kde:dualtree_kde",
               "mlpack/series_expansion:series_expansion",
               "contrib/niche/kmeans_nonempty:kmeans_nonempty"]
    )

binrule(
    name = "test_dna_lmmk",
    sources = ["test_dna_lmmk.cc"],
    headers = ["test_engine.h",
               "test_engine_impl.h",
               "latent_mmk.h",
               "latent_mmk_impl.h",
               "utils.h",
               "multinomial.h",               
               "isotropic_gaussian.h",
               "diag_gaussian.h",
               "mixture.h",
               "hmm.h",
               "../svm/svm.h",
               "../svm/smo.h"],
    deplibs = ["fastlib:fastlib",
               "mlpack/kde:dualtree_kde",
               "mlpack/series_expansion:series_expansion",
               "contrib/niche/kmeans_nonempty:kmeans_nonempty"]
    )

binrule(
    name = "test_dna_gmmk",
    sources = ["test_dna_gmmk.cc"],
    headers = ["test_engine.h",
               "test_engine_impl.h",
               "generative_mmk.h",
               "generative_mmk_impl.h",
               "utils.h",
               "multinomial.h",               
               "isotropic_gaussian.h",
               "diag_gaussian.h",
               "mixture.h",
               "hmm.h",
               "../svm/svm.h",
               "../svm/smo.h"],
    deplibs = ["fastlib:fastlib",
               "mlpack/kde:dualtree_kde",
               "mlpack/series_expansion:series_expansion",
               "contrib/niche/kmeans_nonempty:kmeans_nonempty"]
    )

binrule(
    name = "test_engine",
    sources = ["test_engine.cc"],
    headers = ["test_engine.h",
               "test_engine_impl.h",
               "../svm/smo.h",
               "../svm/svm.h",
               "generative_mmk.h",
               "generative_mmk_impl.h",
               "utils.h",
               "isotropic_gaussian.h",
               "multinomial.h",               
               "hmm.h",
               "diag_gaussian.h",
               "mixture.h"],
    deplibs = ["fastlib:fastlib",
               "mlpack/kde:dualtree_kde",
               "mlpack/series_expansion:series_expansion",
               "contrib/niche/kmeans_nonempty:kmeans_nonempty"]
    )

# The driver for the bandwidth cross-validator.
binrule(
    name = "get_optimal_kde_bandwidth",
    sources = ["get_optimal_kde_bandwidth.cc"],
    headers = [],
    deplibs = ["mlpack/kde:dualtree_kde",
               "mlpack/series_expansion:series_expansion",
               "fastlib:fastlib_int"]
    )


binrule(
    name = "test_isotropic_gaussian",
    sources = ["test_isotropic_gaussian.cc"],
    headers = ["isotropic_gaussian.h"],
    deplibs = ["fastlib:fastlib"]
    )

binrule(
    name = "generative_mmk",
    sources = ["generative_mmk.cc"],
    headers = ["generative_mmk.h",
               "generative_mmk_impl.h",
               "utils.h",
               "isotropic_gaussian.h",
               "multinomial.h",               
               "hmm.h",
               "diag_gaussian.h",
               "mixture.h"],
    deplibs = ["fastlib:fastlib",
               "mlpack/kde:dualtree_kde",
               "mlpack/series_expansion:series_expansion",
               "contrib/niche/kmeans_nonempty:kmeans_nonempty"]
    )

binrule(
    name = "test_latent_mmk",
    sources = ["test_latent_mmk.cc"],
    headers = ["latent_mmk.h",
               "latent_mmk_impl.h",
               "utils.h",
               "hmm.h",
               "multinomial.h",
               "diag_gaussian.h",
               "mixture.h"],
    deplibs = ["fastlib:fastlib",
               "contrib/niche/kmeans_nonempty:kmeans_nonempty"]
    )

binrule(
    name = "latent_mmk",
    sources = ["latent_mmk.cc"],
    headers = ["latent_mmk.h",
               "latent_mmk_impl.h",
               "hmm.h",
               "multinomial.h",
               "diag_gaussian.h",
               "mixture.h"],
    deplibs = ["fastlib:fastlib",
               "contrib/niche/kmeans_nonempty:kmeans_nonempty"]
    )

binrule(
    name = "hmm",
    sources = ["hmm.cc"],
    headers = ["hmm.h",
               "multinomial.h",
               "diag_gaussian.h",
               "mixture.h",
               "utils.h"],
    deplibs = ["fastlib:fastlib",
               "contrib/niche/kmeans_nonempty:kmeans_nonempty"]
    )

binrule(
    name = "tc",
    sources = ["test_template_classes.cc"],
    headers = ["test_template_classes.h",
               "multinomial.h",
               "gaussian.h",
               "mixture.h"],
    deplibs = ["fastlib:fastlib"]
    )

binrule(
    name = "testMatrix",
    sources = ["testMatrix.cc"],
    headers = [],
    deplibs = ["fastlib:fastlib"]
    )

binrule(
    name = "otrav_mem_test",
    sources = ["otrav_mem_test.cc"],
    headers = [],
    deplibs = ["fastlib:fastlib"]
    )

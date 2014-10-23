テンプレート
==============

20年前にPHPが作られた頃、開発者達はPHPのシンプルさや、PHPコードのHTMLとの親和性を愛していました。
しかし、時間が経つとともに、テンプレートをもっとうまく扱うために他のテンプレート言語（例えば `Twig`_  ）が作られるようになりました。

.. best-practice::

    テンプレートには Twig を使いましょう。

一般的に、PHPのテンプレートはTwigよりも冗長になります。というのも、PHPのテンプレートは継承、自動エスケープ、フィルタや関数への名前付き引数などの、最新の機能を備えていないからです。
TwigはSymfonyでのデフォルトのテンプレート言語で、PHP以外も含めた全てのテンプレートエンジンの中で最大のコミュニティサポートがあり、Drupal 8 のような負荷の高いプロジェクトでも使われています。

それに加えて、TwigはSymfony3.0で動作が保証される唯一のテンプレートエンジンです。
実のところ、PHPテンプレートエンジンは、Symfonyの公式なサポートから除外されるでしょう。

テンプレートファイルの配置
----------------------------

.. best-practice::

    アプリケーションのテンプレートファイルは、全て ``app/Resources/views`` ディレクトリに置きましょう。

伝統的に、Symfonyの開発者達はアプリケーションのテンプレートファイルを、それぞれのバンドルの ``Resources/views`` ディレクトリに保存してきました。
そして、その場所を参照する仮想的な名前を使っていました（例えば ``AcmeDemoBundle:Default:index.html.twig`` ）

しかし、アプリケーションで使用するテンプレートについては、 ``app/Resources/views/`` ディレクトリに保存するほうが便利なのです。
初心者にとっては、この保存先を使うことによって、ビューの名前を劇的にシンプルにすることができます。

==================================================  ==================================
バンドルに保存されたテンプレート                     ``app/`` に保存されたテンプレート
==================================================  ==================================
``AcmeDemoBunde:Default:index.html.twig``           ``default/index.html.twig``
``::layout.html.twig``                              ``layout.html.twig``
``AcmeDemoBundle::index.html.twig``                 ``index.html.twig``
``AcmeDemoBundle:Default:subdir/index.html.twig``   ``default/subdir/index.html.twig``
``AcmeDemoBundle:Default/subdir:index.html.twig``   ``default/subdir/index.html.twig``
==================================================  ==================================

もう一つのメリットは、テンプレートを一カ所に集中させることができ、デザイナーの仕事が簡単になることです。多数のバンドル内のディレクトリに分かれたテンプレートを探しまわる必要がなくなるのです。

Twigエクステンション
---------------------

.. best-practice::

    Twigエクステンションは ``AppBundle/Twig/`` ディレクトリに置き、 ``app/config/services.yml`` を使って設定しましょう。

開発中のアプリケーションに、独自の ``md2html`` というTwigフィルタが必要だと想像してください。
Markdown形式のコンテンツをHTMLに変換するフィルタです。

このフィルタを仕える王にするには、まず、Markdownパーサーの `Parsedown`_ をプロジェクトの新しい依存ライブラリとしてインストールします。

.. code-block:: bash

    $ composer require erusev/parsedown

次に、 ``Markdown`` サービスを新しく作成します。後でTwigエクステンションから利用するためのサービスです。
サービスの定義はクラスのパスだけでできます。

.. code-block:: yaml

    # app/config/services.yml
    services:
        # ...
        markdown:
            class: AppBundle\Utils\Markdown

そして、 ``Markdown`` クラスには、MarkdownをHTMLに変換するメソッドが一つだけ必要です。::

    namespace AppBundle\Utils;

    class Markdown
    {
        private $parser;

        public function __construct()
        {
            $this->parser = new \Parsedown();
        }

        public function toHtml($text)
        {
            $html = $this->parser->text($text);

            return $html;
        }
    }

次に、新しいTwigエクステンションを作成し、``Twig_SimpleFilter`` を使って  ``md2html`` という新しいフィルタを作りましょう。
Twigエクステンションには、新しく定義したばかりの ``markdown`` サービスをコンストラクタで注入します。

.. code-block:: php

    namespace AppBundle\Twig;

    use AppBundle\Utils\Markdown;

    class AppExtension extends \Twig_Extension
    {
        private $parser;

        public function __construct(Markdown $parser)
        {
            $this->parser = $parser;
        }

        public function getFilters()
        {
            return array(
                new \Twig_SimpleFilter(
                    'md2html',
                    array($this, 'markdownToHtml'),
                    array('is_safe' => array('html'))
                ),
            );
        }

        public function markdownToHtml($content)
        {
            return $this->parser->toHtml($content);
        }

        public function getName()
        {
            return 'app_extension';
        }
    }


最後に、新しいサービスを定義して、このTwigエクステンションをアプリケーションで利用できるようにします。（このサービスは開発するコードの中では利用しないので、サービス名は何でも構いません）

.. code-block:: yaml

    # app/config/services.yml
    services:
        app.twig.app_extension:
            class:     AppBundle\Twig\AppExtension
            arguments: ["@markdown"]
            tags:
                - { name: twig.extension }


.. _`Twig`: http://twig.sensiolabs.org/
.. _`Parsedown`: http://parsedown.org/
.. _`Twig global variables`: http://symfony.com/doc/master/cookbook/templating/global_variables.html
.. _`override error pages`: http://symfony.com/doc/current/cookbook/controller/error_pages.html
.. _`render a template without using a controller`: http://symfony.com/doc/current/cookbook/templating/render_without_controller.html

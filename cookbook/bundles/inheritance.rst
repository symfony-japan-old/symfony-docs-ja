.. index::
   single: Bundle; Inheritance

バンドルの継承を使って既存のバンドルのパーツを上書きする方法
============================================================

サードパーティのバンドルを使用する際に遭遇する問題として、サードパーティのバンドルのファイルを自分の開発するバンドル内でオーバーライドする必要があるときがあります。 Symfony はコントローラ、テンプレート、翻訳、その他バンドルの ``Resources/`` ディレクトリ内のファイルをオーバーライドする便利な方法を用意しています。

例として、 `FOSUserBundle`_ をインストールして、ベーステンプレートである ``layout.html.twig`` と、コントローラを１つオーバーライドしてみます。また、独自に ``AcmeUserBundle`` を開発しており、ファイルをオーバーライドすることにします。まず、 ``FOSUserBundle`` をあなたのバンドル ``AcmeUserBundle`` の "parent" として登録してください。

::

    // src/Acme/UserBundle/AcmeUserBundle.php
    namespace Acme\UserBundle;

    use Symfony\Component\HttpKernel\Bundle\Bundle;

    class AcmeUserBundle extends Bundle
    {
        public function getParent()
        {
            return 'FOSUserBundle';
        }
    }

このシンプルな変更により、同じ名前のファイルを作成すれば ``FOSUserBundle`` のいくつかの部分をオーバーライドできるようになりました。

コントローラのオーバライド
~~~~~~~~~~~~~~~~~~~~~~~~~~

``FOSUserBundle`` にある ``RegistrationController`` の ``registerAction`` にいくつか機能を追加してみます。必要なことは、独自に ``RegistrationController.php`` ファイルを作成して、同じ名前のメソッドを作り、追加したい機能を変更するだけです。

::

    // src/Acme/UserBundle/Controller/RegistrationController.php
    namespace Acme\UserBundle\Controller;

    use FOS\UserBundle\Controller\RegistrationController as BaseController;

    class RegistrationController extends BaseController
    {
        public function registerAction()
        {
            $response = parent::registerAction();
            
            // do custom stuff
            
            return $response;
        }
    }

.. tip::

    コントローラの内部の変更の内容に寄って、 ``parent::registerAction()`` を呼び変更を加えるか、もしくは、ロジックを全て書き換えるとになります。

.. note::

    バンドルがルーティングルールやテンプレートの中で、コントローラの標準的なシンタックスである ``FOSUserBundle:Registration:register`` を使用しているときのみ、この方法でコントローラをオーバーライドができます。また、これはバンドル作成に関するベストプラクティスです。

リソースのオーバライド: テンプレート、ルーティング、翻訳、バリデーションなど
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ほとんどのリソースは、独自で作成したバンドルを親バンドルとして同じ場所にファイルを作成するだけで、オーバーライドすることができます。

例えば、あなたのアプリケーションのベースレイアウトを使うために、 ``FOSUserBundle`` の ``layout.html.twig`` テンプレートをオーバーライドしたいというニーズは一般的です。このファイルは、 ``FOSUserBundle`` の ``Resources/views/layout.html.twig`` にあるので、 ``AcmeUserBundle`` の同じ場所にファイルを作成すれば、オーバーライドすることができるようになります。そうすることによって、 Symfony は、 ``FOSUserBundle`` 内のベースレイアウトのファイルを無視し、あなたのファイルを使用します。

ルーティングファイル、バリデーションコンフィギュレーション、そして他のリソースに関しても同様です。

.. note::

    ``@FosUserBundle/Resources/config/routing/security.xml`` のようにリソースを指定しているときのみ、リソースのオーバーライドができます。 @BundleName ショートカットを使用せずにリソースを指定することはできません。

.. _`FOSUserBundle`: https://github.com/friendsofsymfony/fosuserbundle

.. 2011/10/26 ganchiku 8c977955dd659249df896e21c4eab044f044e90b


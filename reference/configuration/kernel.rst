.. index::
    single: Configuration reference; Kernel class

.. note::

    * 対象バージョン ： 2.3（2.1以降）
    * 翻訳更新日 ： 2013/11/25

カーネルの設定(例えば AppKernel)
==========================================

設定の中にはカーネルクラス自体によって行うことができるものもあります。（カーネルクラスは通常 ``app/AppKernel.php`` にあります） 設定のカスタマイズは、継承元の :class:`Symfony\\Component\\HttpKernel\\Kernel` クラスの特定のメソッドをオーバーライドすることで実現できます。

設定
-------------

* `キャラクタセット`_
* `カーネル名`_
* `ルートディレクトリ`_
* `キャッシュディレクトリ`_
* `ログディレクトリ`_

キャラクタセット
~~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``UTF-8``

アプリケーションで使用されるキャラクタセットです。キャラクタセットを変更するには、 :method:`Symfony\\Component\\HttpKernel\\Kernel::getCharset` メソッドをオーバーライドし、別のキャラクタセットを返すように変更します。たとえば下記のようにします。::

    // app/AppKernel.php

    // ...
    class AppKernel extends Kernel
    {
        public function getCharset()
        {
            return 'ISO-8859-1';
        }
    }

カーネル名
~~~~~~~~~~~

**type**: ``string`` **default**: ``app`` (カーネルクラスが配置されているディレクトリ名)

設定を変更するには、 :method:`Symfony\\Component\\HttpKernel\\Kernel::getName` メソッドをオーバーライドしてください。或いは、カーネルクラスを他のディレクトリに移動しても良いです。
たとえば、カーネルを ``app`` から ``foo`` ディレクトリに移動すると、カーネル名は ``foo`` になります。

カーネル名は、通常は直接問題になることはありません。キャッシュファイルの生成に使われるだけだからです。
カーネルを複数使用するアプリケーションを作る場合、一番簡単な方法は ``app`` ディレクトリをコピーしてディレクトリ名を変更することです。（ ``foo`` のようなディレクトリ名に）

ルートディレクトリ
~~~~~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``AppKernel`` のディレクトリ

カーネルのルートディレクトリを返します。 Symfony Standard edition の場合、ルートディレクトリは ``app`` ディレクトリを指しています。

設定を変更するには、 :method:`Symfony\\Component\\HttpKernel\\Kernel::getRootDir` メソッドをオーバーライドします。::

    // app/AppKernel.php

    // ...
    class AppKernel extends Kernel
    {
        // ...

        public function getRootDir()
        {
            return realpath(parent::getRootDir().'/../');
        }
    }

キャッシュディレクトリ
~~~~~~~~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``$this->rootDir/cache/$this->environment``

キャッシュディレクトリのパスを返します。設定を変更するには、 :method:`Symfony\\Component\\HttpKernel\\Kernel::getCacheDir` メソッドをオーバーライドしてください。
詳しくは、 ":ref:`override-cache-dir`" を参照してください。


ログディレクトリ
~~~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``$this->rootDir/logs``

ログディレクトリのパスを返します。設定を変更するには、 :method:`Symfony\\Component\\HttpKernel\\Kernel::getLogDir` メソッドをオーバーライドしてください。
詳しくは ":ref:`override-logs-dir`" を参照してください。

.. 2013/11/26 77web 4346f75f05a5ee010d0148ea251e99c7f6a02c38
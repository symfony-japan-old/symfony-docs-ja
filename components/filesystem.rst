.. index::
   single: Filesystem

.. note::

    * 翻訳更新日：2013/03/21


Filesystemコンポーネント
========================

    Filesystemコンポーネントはファイルシステムの基本的なユーティリティです。

.. versionadded:: 2.1
    FilesystemコンポーネントはSymfony2.1から追加されました。以前は ``Filesystem`` クラスは ``HttpKernel`` コンポーネントに含まれていました。

インストール
------------

コンポーネントをインストールする方法は何通りもあります。

* 公式Gitレポジトリ (https://github.com/symfony/Filesystem)
* Composer (`Packagist`_ の ``symfony/filesystem``)

使い方
-----

:class:`Symfony\\Component\\Filesystem\\Filesystem` クラスはファイル操作の唯一のエンドポイントです::

    use Symfony\Component\Filesystem\Filesystem;
    use Symfony\Component\Filesystem\Exception\IOException;

    $fs = new Filesystem();

    try {
        $fs->mkdir('/tmp/random/dir/' . mt_rand());
    } catch (IOException $e) {
        echo "An error occurred while creating your directory";
    }

.. note::

    :method:`Symfony\\Component\\Filesystem\\Filesystem::mkdir`,
    :method:`Symfony\\Component\\Filesystem\\Filesystem::exists`,
    :method:`Symfony\\Component\\Filesystem\\Filesystem::touch`,
    :method:`Symfony\\Component\\Filesystem\\Filesystem::remove`,
    :method:`Symfony\\Component\\Filesystem\\Filesystem::chmod`,
    :method:`Symfony\\Component\\Filesystem\\Filesystem::chown`,
    :method:`Symfony\\Component\\Filesystem\\Filesystem::chgrp` の各メソッドは引数として文字列、配列、または :phpclass:`Traversable` を実装したオブジェクトを取ることができます。


Mkdir
~~~~~

Mkdirはディレクトリを作成します。posixファイルシステムではデフォルトでモード `0777` で作成します。第二引数を利用して任意のモードを指定することができます。::

    $fs->mkdir('/tmp/photos', 0700);

.. note::

    第一引数には配列や :phpclass:`Traversable` オブジェクトも使用できます。

Exists
~~~~~~

Existsはファイルやディレクトリが存在するかどうか調べて、存在しなければfalseを返します。::

    // ディレクトリが存在するのでtrueを返します
    $fs->exists('/tmp/photos');

    // rabbit.jpgは存在しbottle.pngが存在しないのでfalseを返します
    $fs->exists(array('rabbit.jpg', 'bottle.png'));

.. note::

    第一引数には配列や :phpclass:`Traversable` オブジェクトも使用できます。

Copy
~~~~

このメソッドはファイルをコピーするのに使います。もしコピー先のファイルが既に存在していれば、更新日時がコピー先ファイルより新しい場合のみコピーを行います。この挙動は第三引数のbooleanにより変更することができます。::

    // image-ICC.jpg が image.jpg より後に更新された場合のみコピーします
    $fs->copy('image-ICC.jpg', 'image.jpg');

    // image.jpg は常に更新されます
    $fs->copy('image-ICC.jpg', 'image.jpg', true);

Touch
~~~~~

Touchはファイルのアクセス時間と更新時間を更新します。デフォルトでは現在時刻が使われます。更新時間に任意の時間を指定したい場合は第二引数を使ってください。第三引数はアクセス時間です。::

    // 更新時刻を現在時刻に設定します
    $fs->touch('file.txt');
    // 更新時刻を現在より10秒後に設定します
    $fs->touch('file.txt', time() + 10);
    // アクセス時間を現在より10秒前に設定します
    $fs->touch('file.txt', time(), time() - 10);

.. note::

    第一引数には配列や :phpclass:`Traversable` オブジェクトも使用できます。

Chown
~~~~~

Chownはファイルの所有者を変更するのに使います。第三引数はbooleanで、再帰するかどうかを指定できます。::

    // lolcatビデオの所有者をwww-dataに変更します
    $fs->chown('lolcat.mp4', 'www-data');
    // videoディレクトリとその配下のディレクトリ・ファイルの所有者を再帰的にwww-dataに変更します
    $fs->chown('/video', 'www-data', true);

.. note::

    第一引数には配列や :phpclass:`Traversable` オブジェクトも使用できます。

Chgrp
~~~~~

Chgrpはファイルのグループを変更するのに使います。第三引数はbooleanで、再帰するかどうかを指定できます。::

    // lolcatビデオのグループをnginxに変更します
    $fs->chgrp('lolcat.mp4', 'nginx');
    // videoディレクトリとその配下のディレクトリ・ファイルのグループを再帰的にnginxに変更します
    $fs->chgrp('/video', 'nginx', true);


.. note::

    第一引数には配列や :phpclass:`Traversable` オブジェクトも使用できます。

Chmod
~~~~~

Chmodはファイルのモードを変更するのに使います。第三引数はbooleanで、再帰するかどうかを指定できます。::

    // video.oggのモードを0600に変更します
    $fs->chmod('video.ogg', 0600);
    // srcディレクトリとその配下のディレクトリ・ファイルのモードを再帰的にnginxに変更します
    $fs->chmod('src', 0700, 0000, true);

.. note::

    第一引数には配列や :phpclass:`Traversable` オブジェクトも使用できます。

Remove
~~~~~~

Removeを使ってファイル、シンボリックリンク、ディレクトリを簡単に削除しましょう。::

    $fs->remove(array('symlink', '/path/to/directory', 'activity.log'));

.. note::

    第一引数には配列や :phpclass:`Traversable` オブジェクトも使用できます。

Rename
~~~~~~

Renameはファイルやディレクトリの名前を変えるのに使います。::

    // ファイル名を変更します
    $fs->rename('/tmp/processed_video.ogg', '/path/to/store/video_647.ogg');
    // ディレクトリ名を変更します
    $fs->rename('/tmp/files', '/path/to/store/files');

symlink
~~~~~~~

シンボリックリンクを作成します。ファイルシステムがシンボリックリンクをサポートしていない場合は第三引数が役立ちます。::

    // シンボリックリンクを作る
    $fs->symlink('/path/to/source', '/path/to/destination');
    // ファイルシステムがシンボリックリンクをサポートしていない場合
    // /path/to/sourceを/path/to/destinationにコピーします
    $fs->symlink('/path/to/source', '/path/to/destination', true);

makePathRelative
~~~~~~~~~~~~~~~~

ディレクトリのパスを、第二引数のパスに対する相対パスに変更します::

    // returns '../'
    $fs->makePathRelative(
        '/var/lib/symfony/src/Symfony/',
        '/var/lib/symfony/src/Symfony/Component'
    );
    // returns 'videos'
    $fs->makePathRelative('/tmp', '/tmp/videos');

mirror
~~~~~~

ディレクトリをミラーリングします。::

    $fs->mirror('/path/to/source', '/path/to/target');

isAbsolutePath
~~~~~~~~~~~~~~

isAbsolutePathは与えられたパスが絶対パスならtrueを、絶対パスでなければfalseを返します。::

    // return true
    $fs->isAbsolutePath('/tmp');
    // return true
    $fs->isAbsolutePath('c:\\Windows');
    // return false
    $fs->isAbsolutePath('tmp');
    // return false
    $fs->isAbsolutePath('../dir');

エラー処理
--------------

何かエラーが発生したときは :class:`Symfony\\Component\\Filesystem\\Exception\\ExceptionInterface` を実装した例外がスローされます。

.. note::

    2.1より前のバージョンでは :method:`Symfony\\Component\\Filesystem\\Filesystem::mkdir` はbooleanを返し、例外をスローしませんでした。2.1以降ではディレクトリの作成に失敗した場合 :class:`Symfony\\Component\\Filesystem\\Exception\\IOException` がスローされます。

.. _`Packagist`: https://packagist.org/packages/symfony/filesystem

.. 2013/03/21 77web eed8e469e65a8531dceeb7e9c55f107bb1e5bb13